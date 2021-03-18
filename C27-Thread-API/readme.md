# Chapter 27 - Thread API

Q2. What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does helgrind report in each of these cases?

- no error

```c
	1 #include <stdio.h>
  2
  3 #include "common_threads.h"
  4
  5 int balance = 0;
  6 pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  7
  8 void* worker(void* arg) {
  9     Mutex_lock(&lock);
 10     balance++; // unprotected access
 11     Mutex_unlock(&lock);
 12     return NULL;
 13 }
 14
 15 int main(int argc, char *argv[]) {
 16     pthread_t p;
 17     Pthread_create(&p, NULL, worker, NULL);
 18     Mutex_lock(&lock);
 19     balance++; // unprotected access
 20     Mutex_unlock(&lock);
 21     Pthread_join(p, NULL);
 22     return 0;
 23 }
```

Q5. Now run helgrind on main-deadlock-global.c. Examine the code; does it have the same problem that main-deadlock.c has? Should helgrind be reporting the same error? What does this tell you about tools like helgrind?

```c
	1 #include <stdio.h>
  2
  3 #include "common_threads.h"
  4
  5 pthread_mutex_t g = PTHREAD_MUTEX_INITIALIZER;
  6 pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
  7 pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
  8
  9 void* worker(void* arg) {
 10     Pthread_mutex_lock(&g);
 11     if ((long long) arg == 0) {
 12   Pthread_mutex_lock(&m1);
 13   Pthread_mutex_lock(&m2);
 14     } else {
 15   Pthread_mutex_lock(&m2);
 16   Pthread_mutex_lock(&m1);
 17     }
 18     Pthread_mutex_unlock(&m1);
 19     Pthread_mutex_unlock(&m2);
 20     Pthread_mutex_unlock(&g);
 21     return NULL;
 22 }
 23
 24 int main(int argc, char *argv[]) {
 25     pthread_t p1, p2;
 26     Pthread_create(&p1, NULL, worker, (void *) (long long) 0);
 27     Pthread_create(&p2, NULL, worker, (void *) (long long) 1);
 28     Pthread_join(p1, NULL);
 29     Pthread_join(p2, NULL);
 30     return 0;
 31 }
```

- valgrind should not report any error(as there is an additional global lock `g` which would prevent deadlock), but it does.

Q6. Let’s next look at main-signal.c. This code uses a variable (done) to signal that the child is done and that the parent can now continue. Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)

- the parent thread falls in a busy wait status(which still require the cpu resource) and this is inefficient

Q8. Now look at a slightly modified version of the code, which is found in main-signal-cv.c. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?

```c
#include <stdio.h>

#include "common_threads.h"

// 
// simple synchronizer: allows one thread to wait for another
// structure "synchronizer_t" has all the needed data
// methods are:
//   init (called by one thread)
//   wait (to wait for a thread)
//   done (to indicate thread is done)
// 
typedef struct __synchronizer_t {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int done;
} synchronizer_t;

synchronizer_t s;

void signal_init(synchronizer_t *s) {
    Pthread_mutex_init(&s->lock, NULL);
    Pthread_cond_init(&s->cond, NULL);
    s->done = 0;
}

void signal_done(synchronizer_t *s) {
    Pthread_mutex_lock(&s->lock);
    s->done = 1;
    Pthread_cond_signal(&s->cond);
    Pthread_mutex_unlock(&s->lock);
}

void signal_wait(synchronizer_t *s) {
    Pthread_mutex_lock(&s->lock);
    while (s->done == 0)
	Pthread_cond_wait(&s->cond, &s->lock);
    Pthread_mutex_unlock(&s->lock);
}

void* worker(void* arg) {
    printf("this should print first\n");
    signal_done(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    signal_init(&s);
    Pthread_create(&p, NULL, worker, NULL);
    signal_wait(&s);
    printf("this should print last\n");

    return 0;
}
```
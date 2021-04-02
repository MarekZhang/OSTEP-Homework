#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
  int    queue1;
  int    queue2;
  sem_t  *q1;
  sem_t  *q2;
  sem_t  *mutex;
} ns_mutex_t;

ns_mutex_t lock;
const char* sem_q1 = "/sem_q1";
const char* sem_q2 = "/sem_q2";
const char* sem_mutex = "/sem_mutex";

sem_t *Sem_open(const char *name, int val) {
  sem_t *sem = sem_open(name, O_CREAT, S_IRWXU, val);
  if (sem == SEM_FAILED) {
    printf("failed to create a semaphore.\n");
    exit(0);
  }
  return sem;
}

void ns_mutex_init(ns_mutex_t *m) {
  m->queue1 = 0;
  m->queue2 = 0;
  m->q1 = Sem_open(sem_q1, 1);
  m->q2 = Sem_open(sem_q2, 0);
  m->mutex = Sem_open(sem_mutex, 1);
}

void ns_mutex_acquire(ns_mutex_t *m) {
  Sem_wait(m->mutex);
  m->queue1 ++;
  Sem_post(m->mutex); 

  Sem_wait(m->q1);
  m->queue2 ++;
  Sem_wait(m->mutex);
  m->queue1 --;

  if (m->queue1 == 0) {
    Sem_post(m->q2);
  } else {
    Sem_post(m->q1);
  }

  Sem_post(m->mutex);
  
  // use the second queue as the barrier
  Sem_wait(m->q2);
  m->queue2 --;
}

void ns_mutex_release(ns_mutex_t *m) {
  if (m->queue2 == 0) {
   Sem_post(m->q1); 
  } else {
   Sem_post(m->q2);
  }
}


void *worker(void *arg) {
    ns_mutex_acquire(&lock);
    int thread_id = (int) pthread_self();

    if (thread_id % 3 == 0)
      sleep(1);

    printf("%d is running...\n", thread_id);
    ns_mutex_release(&lock);
    return NULL;
}


int main(int argc, char *argv[]) {
    assert(argc == 2);
    int count = atoi(argv[1]);
    pthread_t threads[count];
    ns_mutex_init(&lock);

    int i;
    for (i = 0; i < count; i++ ) {
      Pthread_create(&threads[i], NULL, worker, NULL);
    }

    for(int i = 0; i < count; i++) {
      Pthread_join(threads[i], NULL);
    }
    printf("parent: begin\n");
    printf("parent: end\n");
    Sem_close(lock.q1);
    Sem_close(lock.q2);
    Sem_close(lock.mutex);
    Sem_unlink(sem_q1);
    Sem_unlink(sem_q2);
    Sem_unlink(sem_mutex);
    
    return 0;
}


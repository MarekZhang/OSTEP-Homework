#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
    // add semaphores and other information here
  sem_t     *lock;    
  sem_t     *barr;
  int       thread_count;
  int       thread_arrived;
} barrier_t;


// the single barrier we are using for this program
barrier_t b;
const char * barrier_lock = "/lock";
const char * barrier_barr = "/barr";

sem_t *Sem_open(const char *name, int val) {
  sem_t *sem = sem_open(name, O_CREAT, S_IRWXU, val);
  if (sem == SEM_FAILED) {
    printf("failed to create a semaphore.\n");
    exit(0);
  }
  return sem;
}

void barrier_init(barrier_t *b, int num_threads) {
    // initialization code goes here
  
  b->lock = Sem_open(barrier_lock, 1);
  b->barr = Sem_open(barrier_barr, 0);
  b->thread_count = num_threads;
  b->thread_arrived = 0;
}

void barrier(barrier_t *b) {
    // barrier code goes here
  Sem_wait(b->lock);
  b->thread_arrived++;
  if (b->thread_arrived == b->thread_count) {
    Sem_post(b->barr);
  }
  Sem_post(b->lock);
  Sem_wait(b->barr);
  Sem_post(b->barr);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}

// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}


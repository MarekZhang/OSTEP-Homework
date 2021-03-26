#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <mach/thread_act.h>
#include <assert.h>
#include <unistd.h>
#include "helper_lib.h"

#define NUMCPUS 4

typedef struct __counter_t {
  int             global;         //global count
  pthread_mutex_t glock;          //glbal lock
  int             local[NUMCPUS]; //per-cpu count
  pthread_mutex_t llock[NUMCPUS]; //per-cpu locks
  int             threshold;      //update frequency
} counter_t;

typedef struct __thread_args {
  counter_t *counter;
} thread_args;

void init(counter_t *counter, int threshold);

void update(counter_t *c, int threadID, int amt);

int get(counter_t *c);

void *thread_function(void *args);

int main(int argc, char *argv[]) {
  int threshold, i;
  struct timeval time_before, time_after; 

  for (threshold = 1; threshold < 2048; threshold*=2) {
    counter_t *counter = malloc(sizeof(counter_t)); 
    init(counter, threshold);
    pthread_t *threads = malloc(NUMCPUS * sizeof(pthread_t));
    gettimeofday(&time_before, NULL);
    for (i = 0; i < NUMCPUS; i++) {
      thread_args *args = malloc(sizeof(thread_args));
      args->counter = counter;
      Pthread_create(&threads[i], NULL, thread_function, args);
      free(args);
    }

    for (i = 0; i < NUMCPUS; i++) 
      Pthread_join(threads[i], NULL);
    gettimeofday(&time_after, NULL);
    printf("Time elapsed: %.2fs | Approximate Factor: %4d | Global counter : %d\n\n", time_after.tv_sec - time_before.tv_sec + (time_after.tv_usec - time_before.tv_usec) / ONE_MILLION, threshold, counter->global);
    free(counter);
    free(threads);
  }

  return 0;
}

void init(counter_t *c, int threshold) {
  c->threshold =  threshold;
  c->global = 0;
  Pthread_mutex_init(&c->glock, NULL);
  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->local[i] = 0;
    Pthread_mutex_init(&c->llock[i], NULL);
  }
}

void update(counter_t *c, int threadID, int amt) {
  int cpu = threadID % NUMCPUS;
  Pthread_mutex_lock(&c->llock[cpu]);
  c->local[cpu] += amt;
  if(c->local[cpu] >= c->threshold) {
    //transfer to global (assumes amt > 0)
    Pthread_mutex_lock(&c->glock);
    c->global += c->local[cpu];
    Pthread_mutex_unlock(&c->glock);
    c->local[cpu] = 0;
  }

  Pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
  Pthread_mutex_lock(&c->glock);
  int val = c->global;
  Pthread_mutex_unlock(&c->glock);
  return val;
}

void *thread_function(void *args) {
  int i;
  int threadID = (int)pthread_self();
  thread_args *thread = (thread_args*) args;
  counter_t *c = thread->counter;

  for (i = 0; i < ONE_MILLION; i++) {
    update(c, threadID, 1);
  }

  return (void *) 0;
}

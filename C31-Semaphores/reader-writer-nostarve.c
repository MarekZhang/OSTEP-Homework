#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

//
// Your code goes in the structure and functions below
//

const char* readers = "/mutex_reader";
const char* waiting = "/mutex_waiting";
const char* writelock = "/writelock";

typedef struct __rwlock_t {
  int   readers;
  sem_t *mutex_reader;
  sem_t *mutex_waiting;
  sem_t *writelock;
} rwlock_t;

sem_t *Sem_open(const char *name, int val) {
  sem_t *sem = sem_open(name, O_CREAT, S_IRWXU, val);
  if (sem == SEM_FAILED) {
    printf("failed to create a semaphore.\n");
    exit(0);
  }
  return sem;
}

void rwlock_init(rwlock_t *rw) {
  rw->readers = 0;
  rw->mutex_reader = Sem_open(readers, 1);
  rw->mutex_waiting = Sem_open(waiting, 1);
  rw->writelock = Sem_open(writelock, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
  Sem_wait(rw->mutex_reader);
  //the first reader acquire the lock
  if (rw->readers == 0) { // avoid deadlock
    Sem_wait(rw->writelock);
    rw->readers++;
    Sem_post(rw->mutex_reader);
    return;
  }
  Sem_wait(rw->mutex_waiting);
  rw->readers ++;
  Sem_post(rw->mutex_waiting);
  Sem_post(rw->mutex_reader);
}

void rwlock_release_readlock(rwlock_t *rw) {
  Sem_wait(rw->mutex_reader);
  rw->readers --;
  if(rw->readers == 0) {
    Sem_post(rw->writelock);
  }
  Sem_post(rw->mutex_reader);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
  Sem_wait(rw->mutex_waiting);
  Sem_wait(rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
  Sem_post(rw->mutex_waiting);
  Sem_post(rw->writelock);
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);
    sem_close(lock.mutex_reader);
    sem_close(lock.mutex_waiting);
    sem_close(lock.writelock);
    sem_unlink(readers);
    sem_unlink(waiting);
    sem_unlink(writelock);

    printf("end: value %d\n", value);

    return 0;
}


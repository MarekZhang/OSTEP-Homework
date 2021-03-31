#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "common_threads.h"

sem_t *s; 

void *child(void *arg) {
    printf("child\n");
    // use semaphore here
    sem_post(s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    // init semaphore here
    s = sem_open("/sem", O_CREAT, S_IRWXU, 0); 
    if (s == SEM_FAILED) {
      printf("failed to create semaphore");
      exit(0);
    }
      
    Pthread_create(&p, NULL, child, NULL);
    // use semaphore here
    sem_wait(s);
    printf("parent: end\n");

    Sem_close(s);
    return 0;
}


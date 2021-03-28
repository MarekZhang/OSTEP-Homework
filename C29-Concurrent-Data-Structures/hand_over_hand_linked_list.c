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

typedef struct __node {
  int               val;
  pthread_mutex_t   node_lock;
  struct __node     *next;
} node;

typedef struct __linked_list {
  node              *head;
  pthread_mutex_t   list_lock;
} linked_list;

typedef struct __thread_arg {
  int           count;
  linked_list   *list;
} thread_arg;

int insert(linked_list *list, int val);

int traverse(linked_list *list);

int init_list(linked_list *list);

void *thread_function_insert(void *args);

void *thread_function_traverse(void *args);

int main(int argc, char *argv[]) {
  if(argc != 2) {
    fprintf(stderr, "usage: ./hand_over_hand <count>\n");
    exit(EXIT_FAILURE);
  }
  int count = atoi(argv[1]);

  linked_list *list = malloc(sizeof(linked_list));
  if (list == NULL) {
    fprintf(stderr, "failed to allocate memory for list\n");
  }
  init_list(list);
  
  pthread_t *threads = malloc(sizeof(pthread_t) * NUMCPUS);
  struct timeval time_before, time_after;
  
  // 4 threads insert value to the list
  gettimeofday(&time_before, NULL);
  for (int i = 0; i < NUMCPUS; i++) {
    thread_arg *arg = malloc(sizeof(thread_arg));
    arg->count = count;
    arg->list = list;
    Pthread_create(&threads[i], NULL, thread_function_insert, arg);
    free(arg);
  }

  for (int i = 0; i < NUMCPUS; i++) {
    Pthread_join(threads[i], NULL);
  }
  gettimeofday(&time_after, NULL);
  printf("4 threads | each inserting %d elements to the list | time elapsed %.2fs\n", count, time_after.tv_sec - time_before.tv_sec + (time_after.tv_usec - time_before.tv_usec) / ONE_MILLION);

  // 4 threads traverse the list
  gettimeofday(&time_before, NULL);
  for (int i = 0; i < NUMCPUS; i++) {
    thread_arg *arg = malloc(sizeof(thread_arg));
    arg->list = list;
    arg->count = -1;
    Pthread_create(&threads[i], NULL, thread_function_traverse, arg);
    free(arg);
  }

  for (int i = 0; i < NUMCPUS; i++) {
    Pthread_join(threads[i], NULL);
  }
  gettimeofday(&time_after, NULL);
  printf("4 threads | traversing the list | time elapsed %.2fs\n", time_after.tv_sec - time_before.tv_sec + (time_after.tv_usec - time_before.tv_usec) / ONE_MILLION);
  free(threads);
  
  int len = 0;
  node *cur = list->head;
  while (cur != NULL) {
    len++;
    cur = cur->next;
//    printf("node value: %d\n", cur->val);
  }

  printf("The length of the linked list is %d\n", len);

  return 0;
}

int init_list(linked_list *list) {
  Pthread_mutex_init(&list->list_lock, NULL);

  return 0;
}

void *thread_function_insert(void *args) {
  thread_arg *arg = (thread_arg*) args;
  linked_list *list = arg->list;
  int count = arg->count;
  for(int i = 0; i < count; i++) 
    insert(list, i);    
  
  return (void*) 1;
}

void *thread_function_traverse(void *args) {
  thread_arg *arg = (thread_arg*) args;
  linked_list *list = arg->list;
  traverse(list);

  return (void*) 1;
}

// this lock-coupling function still have data race issue
//int insert(linked_list *list, int val) {
//  node *new = malloc(sizeof(node));
//  if(new == NULL)
//    fprintf(stderr, "failed to allocate memory for node\n");
//  Pthread_mutex_init(&new->node_lock, NULL);
//  new->val = val;
//
//  node *old_head = list->head;
//  while (pthread_mutex_unlock(&list->head->node_lock) != 0) {
//    old_head = list->head;
//  }
//  // potential race condition if we do not lock cur node(the new head)
//  Pthread_mutex_lock(&new->node_lock);
//  //lock the head
//  
//  Pthread_mutex_lock(&old_head->node_lock);
//  new->next = old_head;
//  list->head = new;
//  Pthread_mutex_unlock(&old_head->node_lock);
//
//  Pthread_mutex_unlock(&new->node_lock);
//
//  return 0;
//}
//
int insert(linked_list *list, int val) {
  Pthread_mutex_lock(&list->list_lock);
  node * old = list->head;
  node * new = malloc(sizeof(node));
  new->val = val;
  Pthread_mutex_init(&new->node_lock, NULL);
  list->head = new;
  new -> next = old;
  Pthread_mutex_unlock(&list->list_lock);

  return 0;
}

int traverse(linked_list *list) {
  node *cur = list->head;

  Pthread_mutex_lock(&cur->node_lock);
  while (cur->next != NULL) {
    node *nex = cur->next;
    Pthread_mutex_lock(&nex->node_lock);
    Pthread_mutex_unlock(&cur->node_lock);
    cur = nex;
  }
  Pthread_mutex_unlock(&cur->node_lock);

  return 0;
}

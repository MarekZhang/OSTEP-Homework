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
#define Pthread_mutex_init(m, v)    assert(pthread_mutex_init(m, v) == 0);
#define Pthread_mutex_lock(m)       assert(pthread_mutex_lock(m) == 0);
#define Pthread_mutex_unlock(m)     assert(pthread_mutex_unlock(m) == 0);
#define Pthread_create(u,x,y,z)     assert(pthread_create(u,x,y,z) == 0);
#define Pthread_join(m,v)           assert(pthread_join(m,v)==0);
#define ONE_MILLION 1e6

typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;


typedef struct __counter {
  int count;
  pthread_mutex_t lock;
} counter;

typedef struct __thread_arg {
  int cpu_idx;
  pthread_t p;
  counter *c;
} thread_arg;

void init(counter *c);

void increase_counter(counter *c);

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }

// OS X has no default pthread_setaffinity_np method
int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           cpu_set_t *cpu_set);

//thread function
void *thread_function(void *args);


int main(int argc, char *argv[])
{
  counter *c = malloc(sizeof(counter));
  if(c == NULL) {
    printf("Failed to allocate memory space\n");
    exit(1);
  }
  init(c);
  struct timeval time_before, time_after;
  // hyper threading provides twice as much as the number of cores
  int num_of_core = (int)sysconf(_SC_NPROCESSORS_ONLN) / 2;
  // printf("number of processors: %d\n", num_of_core);

  for(int i = 1; i <= num_of_core; i++) {
    thread_arg * args = malloc(i * sizeof(thread_arg));
    if(args == NULL) {
      printf("Failed to allocate memory space\n");
      exit(1);
    }

    gettimeofday(&time_before, NULL);
    // create threads
    for(int j = 1; j <= i; j++) {
      args[j].cpu_idx = j;
      args[j].c = c;
      Pthread_create(&args[j].p, NULL, thread_function, &args[j]);
    }

    // join thread
    for(int j = 1; j <=i; j++) {
      Pthread_join(args[j].p, NULL);
    }
    gettimeofday(&time_after, NULL);
    printf("Running %d threads..\n", i);
    printf("time elapsed %.2f s\n\n", (time_after.tv_sec - time_before.tv_sec) + (time_after.tv_usec - time_before.tv_usec) / ONE_MILLION);

  }



  return 0;
}

// init the counter struct
void init(counter *c)
{
  c->count = 0;
  Pthread_mutex_init(&c->lock, NULL);
}

// increase counter by 1
void increase_counter(counter *c)
{
  Pthread_mutex_lock(&c->lock);
  c->count += 1;
  Pthread_mutex_unlock(&c->lock);
}

// return the value of counter
int get(counter *c)
{
  Pthread_mutex_lock(&c->lock);
  int res = c->count;
  Pthread_mutex_unlock(&c->lock);

  return res;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           cpu_set_t *cpu_set)
{
  thread_port_t mach_thread;
  int core = 0;

  for (core = 0; core < 8 * cpu_size; core++) {
    if (CPU_ISSET(core, cpu_set)) break;
  }
  //printf("binding to core %d\n", core);
  thread_affinity_policy_data_t policy = { core };
  mach_thread = pthread_mach_thread_np(thread);
  thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                    (thread_policy_t)&policy, 1);
  return 0;
}

void *thread_function(void *args)
{
  // cpu affinity
  thread_arg *arg = (thread_arg *) args;
  cpu_set_t *cpu_set;
  cpu_set = malloc(sizeof(cpu_set_t));
  CPU_ZERO(cpu_set);
  CPU_SET(arg->cpu_idx, cpu_set);
  pthread_setaffinity_np(arg->p, sizeof(size_t), cpu_set);
  
  // increase value
  for(int i = 0; i < ONE_MILLION; i++) 
    increase_counter(arg->c);

  return (void*) 1;
}

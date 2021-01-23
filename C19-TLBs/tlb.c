#define _GNU_SOURCE
#include<pthread.h>
#include<sched.h>
#include<sys/time.h>
#include<unistd.h>
#include<fcntl.h>
#include<strings.h>
#include<stdio.h>
#include<stdlib.h>
#include<mach/thread_act.h>
#define PAGESIZE 4096
#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"

typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }


// OS X has no default pthread_setaffinity_np method
int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           cpu_set_t *cpu_set);

int main(int argc, char * argv[])
{
  if(argc < 2){
    printf("Usage: ./tlb arg1 arg2\n");
    printf("arg1: number of pages\n");
    printf("arg2: number of trails\n"); 
    exit(0);
  }

  // bind program to one core of cpu
  cpu_set_t *cpu_set;
  cpu_set = malloc(sizeof(cpu_set_t));
  CPU_ZERO(cpu_set);
  CPU_SET(0, cpu_set);

  pthread_t p = pthread_self();
  pthread_setaffinity_np(p, sizeof(cpu_set_t), cpu_set);

  // benchmarking
  struct timeval time_before, time_after;
  int jump = PAGESIZE / sizeof(int);
  int NUMPAGES = atoi(argv[1]);
  int NUMTRAILS = atoi(argv[2]);
  int *arr = malloc(NUMPAGES * jump * sizeof(int));
  for(int i = 0; i < NUMPAGES * jump; i++)
    arr[i] = i;
  
  gettimeofday(&time_before, NULL);

  for(int i = 0; i < NUMTRAILS; i++){
    for(int j = 0; j < NUMPAGES * jump; j+=jump)
      arr[j] += 1;
  }

  gettimeofday(&time_after, NULL);

  //time in nano
  long time = (1000000 * (time_after.tv_sec - time_before.tv_sec) + time_after.tv_usec - time_before.tv_usec) * 1000;
  float average = (float)time / jump / NUMTRAILS ;

  printf("Jump %5d pages | time %6.1f nanoseconds\n", NUMPAGES, average);

  exit(0);
  
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


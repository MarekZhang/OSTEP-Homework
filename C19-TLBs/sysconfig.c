#include<stdio.h>
#include<sys/times.h>
#include<sys/time.h>
#include<unistd.h>

int main(void){
  struct tms  tmstart, tmsend; 
  clock_t     start, end;
  int         status;
  
  long clktck;
  clktck = sysconf(_SC_CLK_TCK);

  struct timeval before;
  start = times(&tmstart);
  for( int i = 0; i < 1000; i++){
    gettimeofday(&before, NULL);
  }
  end = times(&tmsend);

  printf("time elapsed %7.9f after 1000 gettimeofday function call\n", (double)(end - start) * 1000000000 / 1000000);
  printf("clocks per sec: %ld\n", clktck);
  printf("page size: %ld\n", sysconf(_SC_PAGESIZE));
 }

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char * argv[]){
    if(argc < 3 )
        printf("Invalid number of arguments\n");

    int mem_size = atoi(argv[1]);
    int run_time = atoi(argv[2]);
    char * arr;
    arr = (char *) malloc(mem_size);
    struct timeval cur_time;
    time_t cur, end;
    gettimeofday(&cur_time, NULL);
    cur = cur_time.tv_sec;
    end = cur + run_time;

    int i = 0;
    while(cur < end){
        for(; i < mem_size; i++)
            arr[i] = 0;
        gettimeofday(&cur_time, NULL);
        cur = cur_time.tv_sec;
        printf("still running...\n");
    }
	free(arr);

    exit(0);
}

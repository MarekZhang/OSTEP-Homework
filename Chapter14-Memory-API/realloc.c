#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

struct node{
    struct node * next;
    int val;
};

int main(int argc, char * argv[])
{
    struct timeval start, end;

    if(argc < 2)
        printf("invalid number of argument\n");
    int size = atoi(argv[1]) * 1000;

    struct node * head, *cur, *tempt;
    cur = head = (struct node *) malloc(sizeof(struct node));
    head->val = 0;

    printf("linking nodes with malloc...\n");
    gettimeofday(&start, NULL);
    for(int i = 0; i < size; i++){
        cur->next = (struct node *) malloc(sizeof(struct node));
        cur = cur->next;
        cur->val = i;
    }
    gettimeofday(&end, NULL);
    printf("time elapsed: %.1f ms\n", ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec) / 1000    .0);
    cur = head;
    while(cur){
        tempt = cur;
        cur = cur->next;
        free(tempt);
    }

    printf("building vector with realloc...\n");
    int * arr;
    arr = (int *) malloc(sizeof(int));
    arr[0] = 0;
    gettimeofday(&start, NULL);
    for(int i = 0; i < size; i++){
        arr = realloc(arr, (i+2) * sizeof(int));
        arr[i + 1] = i + 1;
    }
    gettimeofday(&end, NULL);
    printf("time elapsed: %.1f ms\n", ((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec) / 1000    .0);
    free(arr);

    exit(0);
}


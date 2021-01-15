#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
    int * ptr;
    ptr = (int *) malloc(100 * sizeof(int));
    ptr[9] = 99;
    printf("ptr[9]: %d\n", ptr[9]);
    free(ptr);

    printf("invoke calloc function...\n");
    ptr = (int *) calloc(30, sizeof(int));
    printf("ptr[0]: %d initialized to zero\n", ptr[0]);

    exit(0);
}

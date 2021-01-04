#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
    int x = 10;
    x = 100;
    int rc = fork();
    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        x = 99;
        printf("child process get x: %d\n", x);
        exit(0);
    }else{
        x = 999;
        printf("parent process set x to: %d\n", x);
    }

    return 0;
}
#include "chapter5.h"
#include <signal.h>
#define MAXLINE 1024

int main(void){
    int rc, status, pid;
    int fd[2];
    char line[MAXLINE];
    rc = fork();

    if(pipe(fd) < 0)
        printf("pipe error\n");

    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        if(fork() == 0){
            close(fd[1]);
            printf("receive message from another child (%d)\n", getpid());
            int n = read(fd[0], line, MAXLINE);
            write(STDOUT_FILENO, line, n);
            exit(0);
        }else{
            close(fd[0]);
            write(fd[1], "hello from one child\n", 21);
            exit(0);
        }
    }

}
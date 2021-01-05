#include "chapter5.h"
#define MAXLINE 1024

int main(void){
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if(pipe(fd) < 0)
        printf("pipe error");
    printf("pipe fd[0]: %d\n", fd[0]);
    printf("pipe fd[1]: %d\n", fd[1]);
    if((pid = fork()) < 0)
        printf("fork error");
    else if(pid > 0){
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    }else{
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}
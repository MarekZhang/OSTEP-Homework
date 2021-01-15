#include "chapter5.h"
#include <signal.h>

int main(void){
    int rc, status, pid;
    rc = fork();

    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        close(STDOUT_FILENO);
        printf("child (%d) print to stdandard output.\n", getpid());
        exit(0);
    }
    else{
        printf("Goodbye from parent (%d)\n", getpid());
    }
}
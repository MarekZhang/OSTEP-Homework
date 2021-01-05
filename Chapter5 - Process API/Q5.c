#include "chapter5.h"
#include <signal.h>

int main(void){
    int rc, status, pid;
    rc = fork();

    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        int p = wait(NULL);
        printf("Hello from child (%d), p = %d\n", getpid(), p);
        exit(0);
    }
    else{
        pid = wait(&status); 
        if(WIFEXITED(status))
            printf("normal termination of child (%d), exit status = %d\n", pid, WEXITSTATUS(status));
        printf("Goodbye from parent (%d)\n", getpid());
    }
}
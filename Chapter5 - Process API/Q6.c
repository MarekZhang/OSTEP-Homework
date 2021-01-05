#include "chapter5.h"
#include <signal.h>

int main(void){
    int rc, status, pid;
    rc = fork();

    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        printf("Hello from child (%d)\n", getpid());
        exit(0);
    }
    else{
        pid = waitpid(rc, &status, WUNTRACED); 
        if(WIFEXITED(status))
            printf("normal termination of child (%d), exit status = %d\n", pid, WEXITSTATUS(status));
        printf("Goodbye from parent (%d)\n", getpid());
    }
}
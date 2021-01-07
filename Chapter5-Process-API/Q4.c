#include "chapter5.h"

int main(void){
    int rc;
    rc = fork();
    char * args[] = {"ls", "-l", NULL};
    char * env[] = {NULL};

    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        //execl("/bin/ls", "ls", "-l", (char *) 0);
        //execv("/bin/ls", args);
        //execle("/bin/ls", "ls", "-l", (char *) 0, env);
        //execve("/bin/ls", args, env);
        //execlp("ls", "ls", "-l", (char *) 0);
        execvp("ls", args);
    }
    else{
        sleep(1);
        printf("Goodbye from parent\n");
    }

    return 0;
}
#include "chapter5.h"

int main(void){
    int rc;

    rc = fork();
    
    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        printf("Hello from child\n");
        exit(0);
    }
    else{
        sleep(3);
        printf("Goodbye from parent\n");
    }

    return 0;
}
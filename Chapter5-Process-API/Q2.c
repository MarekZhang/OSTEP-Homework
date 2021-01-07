#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(void){
    int fd, rc;
    fd = open("output.txt", O_WRONLY | O_APPEND);
    char buf1[] = "content1\n";
    char buf2[] = "content2\n";
    
    rc = fork();
    if(rc < 0)
        printf("failed to fork\n");
    else if(rc == 0){
        write(fd, buf1, sizeof(buf1) - 1); //remove '\0'
        exit(0);
    }
    else
        write(fd, buf2, sizeof(buf2) - 1); //remove '\0'

    return 0;
}
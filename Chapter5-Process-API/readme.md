# Chapter 5 Homework

Q1. Basics of `fork()`

```c
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
```

- fork调用一次返回两次，child process会复制parent process全部address space内容到自己到address space上(实际采用的是[copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write)). 由于复制了parent的program counter(%rip),child process 会从调用`fork`之后的instruction开始运行。
- 由于copy-on-write的机制，child process与parent process修改variable不会互相影响
- 不同于variable, child process会与parent process共享disk file (child process)；child process与parent process共享file descriptors.任一process对file的修改，对另一process有相同对影响

Q2. child process 与 parent process共用file descriptor

```c
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
```

Q3. using system call `sleep`

```c
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
```

Q4 [不同的exec()](https://en.wikipedia.org/wiki/Exec_(system_call)#C_language_prototypes) l代表list(以vararg的形式传入，需要添加(char*) 0 作为参数结尾); v代表vector(以char * argv[]的形式传入参数)； e代表environment; p指定filename，只需要user传入executable, 程序自动找到PATH下找对应的程序；argv[0]需要设置为将要运行的executable file name

```c
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
```

Q5 `**pid_t wait(int *statloc)`** staloc需要使用macro `WIFEXITED` ... 查看；

```c
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
```

- `wait` child process 多于一个，当第一个child process terminate就从block 状态返回
- 对于没有child process的线程调用`wait` 会立刻返回，返回值为-1

Q6 `pid_t waitpid(pid_t pid, int *status, int options);` 更加灵活，可wait指定process

```c
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
```

Q7 无结果

```c
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
```

Q8

- `pipe(int fildes[2]);` half-duplex; the output of `df[1]` is the input for `fd[0]`
- 调用`pipe` 将当前process空闲的两个(descriptor number最小的两个, 默认情况下 > 2, 因为0，1，2 分配给了`stdin`, `stdout`, `stderr`)file descriptor存储到`fd[]`中
- pipe的两端需要分别关闭`fd[0]`和`fd[1]`

```c
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
```
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
void handle(int signum) {
    printf("recv signal num %d..\n", signum);
    return;
}
//fifo 测试

int main() {
    pid_t pid;
    char buf[1024];
    int ret;
    ret = mkfifo("my_fifo", 0666);
    if(ret != 0){
        perror("mkfifo");
        exit(1);
    }
    pid = fork();
    if (pid == 0) {
        int fd1 = open("my_fifo", O_RDWR);
        int len = write(fd1, "hello\n", sizeof("hello\n"));
        printf("pid0 write len=%d\n",len);
        int rlen = read(fd1, buf, sizeof(buf));
        printf("pid0 read len=%d\n",rlen);
        write(STDOUT_FILENO, buf, rlen);
    } else {
        int fd2 = open("my_fifo", O_RDWR);
        int len = write(fd2, "world\n", sizeof("world\n"));
       printf("main write len=%d\n",len);
        int rlen = read(fd2, buf, sizeof(buf));
        printf("main read len=%d\n",rlen);
        write(STDOUT_FILENO, buf, rlen);
        wait(NULL);

    }
    return 0;
}

/*int main() {
    signal(SIGPIPE, &handle);
    char buf[1024] = {0};
    char buf2[1] = {0};
    int fd[2];

    int pipeRes = pipe(fd);

    if (pipe(fd) == -1)
        sys_err("pipe");
    int i;
    close(fd[0]);
    for (i=0;i<100;i++) {
        printf("times=%d\n", i);
        int writeLen = write(fd[1], buf2, 1);


    }
    return 0;
}*/

/*int main(void)
{
    pid_t pid;
    char buf[1024];
    int fd[2];
    char *p = "test for pipe\n";

    if (pipe(fd) == -1)
        sys_err("pipe");

    pid = fork();
    if (pid < 0) {
        sys_err("fork err");
    } else if (pid == 0) {
        close(fd[1]);
        int len = read(fd[0], buf, sizeof(buf));
        printf("read len=%d\n", len);
        write(STDOUT_FILENO, buf, len);
        close(fd[0]);
    } else {
        close(fd[0]);
        int writeLen = write(fd[1], p, strlen(p));
        printf("writeLen=%d\n", writeLen);
        wait(NULL);
        close(fd[1]);
    }

    return 0;
}*/

/*int main(void)
{
    pid_t pid;
    int fd[2], i, n;
    char buf[1024];

    int ret = pipe(fd);
    if(ret == -1){
        perror("pipe error");
        exit(1);
    }

    for(i = 0; i < 2; i++){
        if((pid = fork()) == 0)
            break;
        else if(pid == -1){
            perror("pipe error");
            exit(1);
        }
    }

    if (i == 0) {
        //只是关闭一个引用，有没有读进程就看有没有持有读的fd 管道的内部结构中也会记录持有的数量
        close(fd[0]);
        int writeLen1 = write(fd[1], "1.hello\n", strlen("1.hello\n"));
        printf("process %d write len=%d\n", i,writeLen1);
    } else if(i == 1) {
        close(fd[0]);
        int writeLen2 = write(fd[1], "2.world\n", strlen("2.world\n"));
        printf("process %d write len=%d\n", i,writeLen2);
    } else {
        close(fd[1]);       //父进程关闭写端,留读端读取数据
//		sleep(1);
        n = read(fd[0], buf, 1024);     //从管道中读数据
        printf("main process read len=%d\n",n);
        write(STDOUT_FILENO, buf, n);
        for(i = 0; i < 2; i++)		//两个儿子wait两次
            wait(NULL);
        n = read(fd[0], buf, 1024);     //从管道中读数据
        printf("main process read len=%d\n",n);
        write(STDOUT_FILENO, buf, n);
        close(fd[0]);
    }

    return 0;
}*/





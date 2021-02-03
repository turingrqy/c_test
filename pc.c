#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>       //for mmap
#include <stdint.h>         //for uint32_t
#include <errno.h>
#include <linux/futex.h>    //for FUTEX_WAIT FUTEX_WAKE
#include <sys/time.h>       //for struct timespec
#include <sys/syscall.h>    //for SYS_futex

int main() {
    pid_t pid;
    char buf[10] = {'a'};
    char **a = &buf;
    pid = fork();
    if (pid == 0) {
        *a = "bbbbbbbbbb";
        printf("child str=%s\n",*a);
        printf("child 指针（地址）的值为：OX%p\n",*a);
    } else {
        printf("main start\n");
        wait(NULL);
        printf("main str=%s\n",*a);
        printf("child 指针（地址）的值为：OX%p\n",*a);
    }
    return 0;
}
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
    int a = 10;
    pid = fork();
    if (pid == 0) {
        printf("child start\n");
        a = 12;
        printf("child int=%d\n",a);
    } else {
        printf("main start\n");
        wait(NULL);
        printf("main int=%d\n",a);
    }
    return 0;
}
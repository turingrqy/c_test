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


void futex_wait(int *futexp)
{
    while(1) {
        //通过一个循环来避免多个线程占用同一个futex的情况
        if(__sync_bool_compare_and_swap(futexp, 1, 0)){
            break;
        }

        int s = futex(futexp, FUTEX_WAIT, 0, NULL, NULL, 0);
        //判断被唤醒后,再占有futex，此时futexp是否会被futex改为0
        printf("futexp = %d\n", *futexp);
        if(s == -1) {
            error_handle("futex: ");
        }
    }
}
void futex_post(int* futexp)
{
    if(__sync_bool_compare_and_swap(futexp, 0, 1)) {
        //这里有一个情况是，将futex值加1后，还没进行下一步唤醒，有新线程过来
        //则新线程会获得futex的所有权
        int s = futex(futexp, FUTEX_WAKE, 1, NULL , NULL, 0);
        if(s == -1) {
            error_handle("futex FUTEX_WAKE:");
        }
    }
}

int main() {
    int *futex = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                               MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int *testNumber = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                                  MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *futex = 1;
    *testNumber = 0;
    pid_t pid;
    int a = 10;
    pid = fork();
    if (pid == 0) {
        int i;
        for( i=0;i<10;i++) {
            futex_wait(futex);
            printf("child in\n");
            *testNumber =*testNumber + 1;
            futex_post(futex);
            printf("child out\n");
        };

    } else {
        int i;
        for( i=0;i<10;i++) {
            futex_wait(futex);
            printf("main in\n");
            *testNumber =*testNumber + 1;
            futex_post(futex);
            printf("main out\n");
        };
        wait(NULL);
    }
    printf("testNumber = %d\n",*testNumber);
    return 0;
}
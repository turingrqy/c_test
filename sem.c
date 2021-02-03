#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>       //for mmap
#include <stdint.h>         //for uint32_t
#include <errno.h>
#include <linux/futex.h>    //for FUTEX_WAIT FUTEX_WAKE
#include <sys/time.h>       //for struct timespec
#include <sys/syscall.h>    //for SYS_futex


int main() {
    pid_t pid;
    int i = 0;
    for(i = 0; i < 3; i++){
        if((pid = fork()) == 0)
            break;
        else if(pid == -1){
            perror("pipe error");
            exit(1);
        }
    }
    if (i==0) {
        sem_t *sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 2);
        sem_wait(sem);
        printf("process id=%d in \n",i);
        sem_post(sem);
        sleep(i);
        printf("process id=%d out \n",i);
    } else if (i==1) {
        sem_t *sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 2);
        sem_wait(sem);
        printf("process id=%d in \n",i);
        sem_post(sem);
        sleep(i);
        printf("process id=%d out \n",i);
    } else if (i==2) {
        sem_t *sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 2);
        sem_wait(sem);
        printf("process id=%d in \n",i);
        sem_post(sem);
        sleep(i);
        printf("process id=%d out \n",i);
    } else {
        sem_t *sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 2);
        sem_wait(sem);
        printf("main in \n",i);
        sem_post(sem);
        printf("main out \n",i);
        wait(NULL);
        sem_close(sem);
        sem_unlink("name_sem");
    }

}
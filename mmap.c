_GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


int main(void) {
    /*int fd;
    pid_t pid;
    char *p_map;
    fd = open("./test.log",O_RDWR);
    p_map = (char *)mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    write(STDOUT_FILENO, p_map, 10);
    pid = fork();
    if (pid == 0) {
        int fd1;
        fd1 = open("./test.log",O_RDWR|O_DIRECT);
        write(fd1, "aaaaaaaaaa", 10);
    } else {

        wait(NULL);
        write(STDOUT_FILENO, "after is:", 9);
        write(STDOUT_FILENO, p_map, 10);
        /*int j;
        for (j=0;j<10;j++) {
            p_map[j] = 'b';

        }
        sync();
    }*/
    int fd1;
    fd1 = open("./test.log",O_RDWR|O_DIRECT);
    write(fd1, "aaaaaaaaaa", 10);

}
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#define DEFAULT_PORT 8888

int main() {
    int socket_fd;
    struct sockaddr_in servaddr;

    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("10.227.8.96");//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
    servaddr.sin_port=htons(DEFAULT_PORT);
    if(connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        return -2;
    }

    while (1) {
        int count;
        char buf[20] = "hello from client!!!";
        count = write(socket_fd,buf,20);
        if (count < 0) {
            perror ("read");
            printf ("Closed connection on descriptor %d,errno=%d errmsg=%s\n",
                    socket_fd,errno,strerror(errno));
            close (socket_fd);
        }
        sleep(3);
    }
}
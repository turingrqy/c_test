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
#include <sys/epoll.h>
#define DEFAULT_PORT 8888

int make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

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
    int connectRes
    connectRes = connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr))
    printf("connect res=%d",connectRes)
    if( connectRes< 0)
    {

        perror("connect");
        return -2;
    }
    make_socket_non_blocking(socket_fd);
    int count;
    char buf[20] = "hello from client!!!";
    count = write(socket_fd,buf,20);
    printf("send to server count=%d\n",count);
    /*while (1) {
        int count;
        char buf[20] = "hello from client!!!";
        count = write(socket_fd,buf,20);
        printf("send to server count=%d\n",count);
        if (count < 0) {
            perror ("read");
            printf ("Closed connection on descriptor %d,errno=%d errmsg=%s\n",
                    socket_fd,errno,strerror(errno));
            close (socket_fd);
        }
        sleep(3);
    }*/
    struct epoll_event event;
    struct epoll_event *events;
    int efd;
    efd = epoll_create1 (0);
    event.data.fd = socket_fd;
    event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;
    int res = epoll_ctl (efd, EPOLL_CTL_ADD, socket_fd, &event);
    if (res == -1)
    {
        printf("epoll_ctl error:%s(errno:%d)\n",strerror(errno),errno);
        perror ("epoll_ctl");
        abort ();
    }
    int n;
    n = epoll_wait (efd, events, 64, -1);
    if (n < 0) {
        printf("epoll_wait error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    if (events[0].events & EPOLLERR) {
        printf("fd = %d, catch EPOLLERR");
        close (events[0].data.fd);
        return 1;
    }
    if (events[0].events & EPOLLHUP) {
        printf("fd = %d, catch EPOLLHUP");
        close (events[0].data.fd);
        return 1;
    }
    if (!(events[0].events & EPOLLIN)) {
        printf("fd = %d, catch event not EPOLLIN event=%d",events[0].events);
        close (events[0].data.fd);
        return 1;
    }
}
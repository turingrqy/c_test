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
    int socket_fd,socket_fd1;
    struct sockaddr_in servaddr;

    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }
    if((socket_fd1=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("10.227.8.96");//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
    servaddr.sin_port=htons(DEFAULT_PORT);
    int connectRes;
    connectRes = connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("connect res=%d",connectRes);
    if( connectRes< 0)
    {

        perror("connect");
        return -2;
    }
    connectRes = connect(socket_fd1, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("connect res=%d",connectRes);
    if( connectRes< 0)
    {

        perror("connect");
        return -2;
    }
    make_socket_non_blocking(socket_fd);
    make_socket_non_blocking(socket_fd1);
    //int count;
    //char buf[20] = "hello from client!!!";
    //count = write(socket_fd,buf,20);
    //printf("send to server count=%d\n",count);

    struct epoll_event event,event1;
    struct epoll_event *events;
    int efd;
    events = calloc (64, sizeof event);
    efd = epoll_create1 (0);
    event.data.fd = socket_fd;
    event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;
    event1.data.fd = socket_fd1;
    event1.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;
    int res = epoll_ctl (efd, EPOLL_CTL_ADD, socket_fd, &event);
    printf("add fd=%d to epoll res=%d", socket_fd,res);
    if (res == -1)
    {
        printf("epoll_ctl error:%s(errno:%d)\n",strerror(errno),errno);

    }
    res = epoll_ctl (efd, EPOLL_CTL_ADD, socket_fd1, &event1);
    printf("add fd=%d to epoll res=%d", socket_fd1,res);
    if (res == -1)
    {
        printf("epoll_ctl error:%s(errno:%d)\n",strerror(errno),errno);

    }

    int count;
    char buf[1400] = {0};
    count = write(socket_fd,buf,1400);
    printf("send to server count=%d\n",count);
    if (count < 0) {
        perror ("read");
        printf ("Closed connection on descriptor %d,errno=%d errmsg=%s\n",
                        socket_fd,errno,strerror(errno));
                close (socket_fd);
    }

        printf("before epoll_wait error:%s(errno:%d)\n",strerror(errno),errno);

        int n;
    while (1) {
        n = epoll_wait (efd, events, 64, -1);
        printf("n:%d in wait\n",n);
        if (n < 0) {
            printf("epoll_wait error:%s(errno:%d)\n",strerror(errno),errno);
            exit(0);
        }
        int i;
        for (i=0;i<n;i++) {


            if (events[i].events & EPOLLERR) {
                printf("fd = %d, catch EPOLLERR",events[0].data.fd);

            }
            if (events[i].events & EPOLLHUP) {
                printf("fd = %d, catch EPOLLHUP",events[0].data.fd);

            }
            if (!(events[i].events & EPOLLIN)) {
                printf("fd = %d, catch event not EPOLLIN event=%d",events[0].data.fd,events[0].events);

            }
            close(events[i].data.fd);
        }
        sync();
    }

    return 1;
}
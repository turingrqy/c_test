#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
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
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
    if((socket_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("create socket error:%s(errno :%d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("10.227.8.96");//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
    servaddr.sin_port=htons(DEFAULT_PORT);

    if(bind(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
    {
        printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    //开始监听是否有客户端连接，第二个参数是最大监听数
    if(listen(socket_fd,10)==-1)
    {
        printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);

    }
    events = calloc (64, sizeof event);
    event.data.fd = socket_fd;
    event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;
    while (1) {
        n = epoll_wait (efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++) {
            if (events[i].events & EPOLLERR) {
                printf("fd = %d, catch EPOLLERR");
                close (events[i].data.fd);
                continue;
            }
            if (events[i].events & EPOLLHUP) {
                printf("fd = %d, catch EPOLLHUP");
                close (events[i].data.fd);
                continue;
            }
            if (!(events[i].events & EPOLLIN)) {
                printf("fd = %d, catch event not EPOLLIN event=%d",events[i].events);
                close (events[i].data.fd);
                continue;
            }
            if (events[i].data.fd == socket_fd) {
                while (1) {
                    infd = accept (sfd, NULL, NULL);
                    if (infd == -1)
                    {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                        {
                            break;
                        } else {
                            perror ("accept");
                            break;
                        }
                    }
                    s = make_socket_non_blocking(infd);
                    if (s == -1) {
                        abort();
                    }
                    event.data.fd = infd;
                    event.events = EPOLLIN|EPOLLERR|EPOLLHUP;
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1) {
                        perror ("epoll_ctl");
                        abort ();
                    }
                }
            }

        } else {
            int count = 0;
            char buf[1024];
            count = read (events[i].data.fd, buf, 1024);
            if (count == -1) {
                if (errno != EAGAIN) {
                    perror ("read");
                    printf ("Closed connection on descriptor %d,errno=%d errmsg=%s\n",
                            events[i].data.fd,errno,strerror(errno));
                    close (events[i].data.fd);
                }
            } else if (count == 0) {
                printf ("Closed connection on descriptor %d due to read count=0\n",
                        events[i].data.fd);
                close (events[i].data.fd);
            }
            printf("recv from client fd=%d,msg=%s",events[i].data.fd, buf);
        }
    }
}

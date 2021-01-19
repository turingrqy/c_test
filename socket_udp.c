#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void udp_msg_sender(int fd, struct sockaddr* dst)
 {

    socklen_t len;
    char buf[10] = "abcdefg";
    len = sizeof(*dst);
    printf("client:%s\n",buf);  //打印自己发送的信息
    sendto(fd, buf, 10, 0, dst, len);
    sendto(fd, buf, 10, 0, dst, len);
 }

void handle_udp_msg(int fd)
{
    char buf[2];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息

    memset(buf, 0, 2);
    len = sizeof(clent_addr);
    count = recvfrom(fd, buf, 2, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
    if(count == -1)
    {
        printf("recieve data fail!\n");
        return;
    }
    printf("count =%d, client:%s\n",count,buf);  //打印client发过来的信息
    memset(buf, 0, 2);
    count = recvfrom(fd, buf, 2, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
    if(count == -1)
    {
        printf("recieve data fail!\n");
        return;
    }
    printf("count =%d, client:%s\n",count,buf);  //打印client发过来的信息
}


int main() {
    pid_t pid;
    int server_fd, ret;
    struct sockaddr_in ser_addr;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(8888);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return -1;
    }
    pid = fork();
    if (pid == 0) {
        int client_fd;
        struct sockaddr_in ser_addr;
        client_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(client_fd < 0) {
            printf("create socket fail!\n");
            return -1;
        }
        memset(&ser_addr, 0, sizeof(ser_addr));
        ser_addr.sin_family = AF_INET;
        //ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
        ser_addr.sin_port = htons(8888);  //注意网络序转换
        socklen_t len;
        udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);
    } else {
        wait(NULL);
        handle_udp_msg(server_fd);
    }

    return 0;
}


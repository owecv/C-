#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>//打印错误信息的头文件
#include <sys/types.h>
#include <fcntl.h>

#define IP "192.168.188.128"
#define PORT 6000
#define STDIN 0

int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);//调用socket函数创建套接字
    assert(sockfd!=-1);

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=inet_addr(IP);
    int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res!=-1);

    int fd=STDIN;
    fd_set fdset;
    struct timeval timeout={5,0};
    while(1)
    {
        //发送数据
        FD_ZERO(&fdset);
        FD_SET(fd,&fdset);//循环添加所有描述符
        FD_SET(sockfd,&fdset);

        int n=select(sockfd+2,&fdset,NULL,NULL,&timeout);
        if(n==-1)
        {
            printf("selection failure\n");
            break;
        }
        else if(n==0)
        {
            //printf("time out\n");
        }
        //对于可读事件，采用普通的recv函数读取数据
        if(FD_ISSET(fd,&fdset))
        {
            printf("send:");
            char send_buff[128]={0};//128字节的发送缓冲区
            fgets(send_buff,128,stdin);
            send(sockfd,send_buff,strlen(send_buff),0);

            if(strncmp(send_buff,"end",3)==0)
            {
                exit(0);
            }
            memset(send_buff,0,128);
        }

        //接收数据

        //对于可读事件，采用普通的recv函数读取数据

        if(FD_ISSET(sockfd,&fdset))

        {
            char recv_buff[128]={0};//128字节的接收缓冲区
            int ret=recv(sockfd,recv_buff,128,0);
            {
                if(ret<=0)
                {
                    break;
                }
            }
            printf("recv:%s",recv_buff);
            fflush(stdout);
            memset(recv_buff,0,128);
        }
    }
    close(sockfd);
}

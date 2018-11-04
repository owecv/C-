#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SER_IP "192.168.188.128"
#define SER_PORT 6000
#define STDIN 0

int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd!=-1);
    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(SER_PORT);//1024 知名端口，1025-4096，临时端口
    saddr.sin_addr.s_addr=inet_addr(SER_IP);

    int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));//命名套接字
    assert(res!=-1);
    listen(sockfd,5);//创建监听队列
    while(1)
    {
        int len=sizeof(caddr);
        int c=accept(sockfd,(struct sockaddr*)&caddr,&len);//接收客户端链接
        if(c<0)
        {
            continue;
        }
        printf("accept c=%d,ip=%s,port:%d\n",c,inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));

        int fd=STDIN;
        fd_set fdset;

        struct timeval timeout={5,0};

        while(1)
        {
            FD_ZERO(&fdset);
            FD_SET(c,&fdset);//循环添加所有描述符
            FD_SET(fd,&fdset);//将键盘也添加进来

            int n=select(c+2,&fdset,NULL,NULL,&timeout);
            if(n==-1)
            {
                printf("selection failure\n");
                break;
            }
            else if(n==0)
            {
                //printf("time out\n");
            }
            if(FD_ISSET(c,&fdset))
            {
                char recv_buff[128]={0};
                if(recv(c,recv_buff,127,0)<=0)
                {
                    break;
                }
                printf("recv:%s",recv_buff);
            }
 
            if(FD_ISSET(fd,&fdset))
            {
                printf("send:");
                char send_buff[128]={0};
                fgets(send_buff,128,stdin);
                send(c,send_buff,sizeof(send_buff),0);
            }
        }
        close(c);
    }
}

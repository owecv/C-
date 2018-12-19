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

//json头文件
#include <iostream>
#include <json/json.h>
using namespace std;
using namespace Json;

#define IP "192.168.126.128"
#define PORT 6000
#define STDIN 0

//枚举出所有的操作
enum _TYPE
{
    TYPE_LOGIN, //登陆                  0
    TYPE_REG,   //注册                  1
    TYPE_ONE,   //一对一聊天            2
    TYPE_GROUD, //群聊                  3
    TYPE_LIST,  //列出所有在线的用户    4
    TYPE_GO     //下线                  5
}TYPE;

void Login(int fd);

void run(int fd)
{
    cout<<"请选择要进行的操作："<<endl;
    cout<<"1.login\n2.register\n3.exit\n"<<endl;
    int choice;
    cin>>choice;
    switch(choice)
    {
        case 1:
        {
            Login(fd);
        }break;
        case 2:
        {
            //Register(fd);
            ;
        }break;
        case 3:
        {
            //Goaway(fd);
            ;
        }break;
        default:
        {
            cout<<"cin error"<<endl;
        }
    }
}

void Login(int fd)
{
    char name[20]="";
    cout<<"please cin name:";

    cin>>name;
    char pw[20]="";
    cout<<"please cin passwd:";
    cin>>pw;

    TYPE=TYPE_LOGIN;
    Json::Value val;
    val["type"]=TYPE;
    val["name"]=name;
    val["passwd"]=pw;

    if(-1==send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0))
    {
        cout<<"send reason fail!"<<endl;
        return ;
    }

    //对从服务器返回的数据进行处理
    //返回“OK”表示存在此用户，登录成功
    char recvbuff[10]="";
    //接收到的数据有误
    if(recv(fd,recvbuff,9,0)<=0)
    {
        cout<<"server unlink or error!"<<endl;
    }

    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"login success!"<<endl;
        //login_success(fd);
    }
    else
    {
        cout<<"no success!"<<endl;
    }
}

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

        run(sockfd);

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

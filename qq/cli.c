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

#if 0
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
#endif

//枚举出所有的操作
enum _TYPE
{   
    TYPE_LOGIN, //登陆                  0   0
    TYPE_REG,   //注册                  1   1
    TYPE_GOAway,//下线                  5   2  5->2  @
    TYPE_LIST,  //列出所有在线的用户    4   3  4->3
    TYPE_ONE,   //一对一聊天            2   4  2->4
    TYPE_GROUD  //群聊                  3   5  3->5
}TYPE;

void Login(int fd);//客户端登录操作
void Register(int fd);//客户端注册操作
void GoAway(int fd);//客户端下线操作

void Login_success(int fd);//当服务器端反馈登录成功时，调用此函数进行处理

void run(int fd)
{
    cout<<"请选择要进行的操作："<<endl;
    cout<<"1.登录\n2.注册\n3.退出\n"<<endl;
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
            Register(fd);
        }break;
        case 3:
        {
            GoAway(fd);
        }break;
        default:
        {
            cout<<"您的输入有误！请重新选择："<<endl;
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

    //制作请求登录的Json包
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

    cout<<"登录时，客户端收到的服务器端的鉴别结果："<<endl<<recvbuff<<endl;
    fflush(stdout);


    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"Login success!"<<endl;
        //登录成功，获取用户在线列表
        Login_success(fd);
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"Login failed!"<<endl;
    }
}

void Register(int fd)//客户端注册操作
{
    char name[20]="";
    cout<<"请输入注册用户的名字：";

    cin>>name;
    char pw[20]="";
    cout<<"密码：";
    cin>>pw;

    //制作请求注册的Json包
    TYPE=TYPE_REG;//注册
    Json::Value val;
    val["type"]=TYPE;
    val["name"]=name;
    val["passwd"]=pw;

    if(-1==send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().    c_str()),0))
    {
        cout<<"发送注册新用户的Json包失败！"<<endl;
        return ;
    }

    //对从服务器返回的数据进行处理
    //返回“OK”表示注册新用户成功
    char recvbuff[10]="";
    //接收到的数据有误

    if(recv(fd,recvbuff,9,0)<=0)
    {
        cout<<"server unlink or error!"<<endl;
    }

    cout<<"注册时，客户端收到的服务器端的鉴别结果："<<endl<<recvbuff<<endl;
    fflush(stdout);

    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"Register success!"<<endl;
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"Register failed!"<<endl;
    }
}

void GoAway(int fd)//客户端下线操作
{
    Json::Value val;
    TYPE=TYPE_GOAway;//用户下线
    val["type"]=TYPE;
    if(-1==send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0))
    {
        cout<<"send json error"<<endl;
        return;
    }

    //对从服务器返回的确认信息进行处理
    //返回“OK”表示该用户下线成功
    char recvbuff[10]="";
    //接收到的数据有误

    if(recv(fd,recvbuff,9,0)<=0)
    {
        cout<<"server unlink or error!"<<endl;
    }

    cout<<"下线操作时，客户端收到的服务器端的鉴别结果："<<endl<<recvbuff<<endl;
    fflush(stdout);

    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"GoAway success!"<<endl;
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"GoAway failed!"<<endl;
    }
}

void Login_success(int fd)//当服务器端反馈登录成功时，调用此函数进行处理
{
    cout<<"请选择要进行的操作："<<endl;
    cout<<"1：获取在线用户列表"<<endl;
    cout<<"2：一对一聊天"<<endl;
    cout<<"3：群聊"<<endl;

    int  choice;
    cin>>choice;

    switch(choice)
    {
        case 1://获取在线用户列表
        {
            TYPE=TYPE_LIST;
            cout<<"TYPE"<<TYPE<<endl;

            Json::Value val;
            val["type"]=TYPE;

            send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
            
            cout<<"等待服务器端确认..."<<endl;
            char buff[128]="";
            if(0<recv(fd,buff,127,0))
            {
                cout<<"在线用户有："<<endl<<buff<<endl;
            }
        }break;
        case 2://一对一聊天
        {
            TYPE=TYPE_LIST; 
            Json::Value val;
            val["type"]=TYPE;
            send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);    

            char buff[128]="";
            if(0<recv(fd,buff,127,0))
            {
                cout<<buff<<endl;
            }
            TYPE=TYPE_ONE;
            Json::Value raw;
            raw["type"]=TYPE;
            string data;
            string name;
            cout<<"please put data"<<endl;
            cin>>data;
            raw["data"]=data;
            cout<<"please put name"<<endl;
            cin>>name;
            raw["name"]=name;
            int fd;
            cin>>fd;
            raw["fd"]=fd;
            send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);    
        }break;
        case 3://群聊
        {
            TYPE=TYPE_GROUD;
            Json::Value val;
            val["type"]=TYPE;
            string data;
            cin>>data;
            val["data"]=data;

            send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);    
        }break;
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

    while(1)
    {
        run(sockfd);
    }
    close(sockfd);
}

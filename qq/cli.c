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
#include <string>

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
#if 0
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
#endif
//枚举出所有的操作
enum _TYPE
{
    TYPE_LOGIN, //登陆                  0   0           0   
    TYPE_REG,   //注册                  1   1           1
    TYPE_EXIT,  //退出                                  2   NULL->2
    TYPE_GOAway,//下线                  5   2  5->2  @  3   2->3
    TYPE_LIST,  //列出所有在线的用户    4   3  4->3     4   3->4
    TYPE_ONE,   //一对一聊天            2   4  2->4     5   4->5
    TYPE_GROUD  //群聊                  3   5  3->5     6   5->6
}TYPE;

string user_name;//保存用户名

void Login(int fd);//客户端登录操作
void Register(int fd);//客户端注册操作
void Exit(int fd);//客户端退出操作
void GoAway(int fd);//客户端下线操作
void ChatToOne(int fd);//一对一聊天操作

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
            Exit(fd);
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

    user_name=name;

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

    if(-1==send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0))
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
        run(fd);
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"Register failed!"<<endl;
    }
}

void Exit(int fd)//客户端退出操作
{
    Json::Value val;
    TYPE=TYPE_EXIT;//用户下线
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

    cout<<"退出操作时，客户端收到的服务器端的反馈结果："<<endl<<recvbuff<<endl;
    fflush(stdout);

    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"EXIT success!"<<endl;
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"EXIT failed!"<<endl;
    }

    exit(0);
}

void GoAway(int fd)//客户端下线操作
{
    Json::Value val;
    TYPE=TYPE_GOAway;//用户下线
    val["type"]=TYPE;
    val["name"]=user_name;
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
        exit(0);
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"GoAway failed!"<<endl;
    }
}

void ChatToOne(int fd)//处理客户端“一对一”聊天请求
{
    cout<<"一对一聊天："<<endl;
    cout<<"请输入对方的ID：";
    int ID;
    cin>>ID;

    //制作请求“一对一聊天”的Json包
    TYPE=TYPE_ONE;//一对一聊天
    Json::Value val;
    val["type"]=TYPE;
    val["ID"]=ID;//对方的ID

    if(-1==send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0))
    {
        cout<<"send reason fail!"<<endl;
        return ;
    }

    //对从服务器返回的数据进行处理
    //返回“OK”表示存在此用户且该用户在线，服务器准备成功
    char recvbuff[10]="";

    //接收到的数据有误
    if(recv(fd,recvbuff,9,0)<=0)
    {
        cout<<"error!"<<endl;
    }

    cout<<"一对一时，客户端收到的服务器端的鉴别结果："<<endl<<recvbuff<<endl;
    fflush(stdout);


    //对返回的数据进行判断
    if(strncmp(recvbuff,"OK",2)==0)
    {
        cout<<"Connect to <<ID<<success!"<<endl;
        //连接成功
        cout<<"send to "<<ID<<":";
        char send_message[1024]="";
        cin>>send_message;
        
        if(-1==send(fd,send_message,strlen(send_message),0))
        {
            cout<<"发送消息失败！"<<endl;
            return;
        }
        else
        {
            cout<<"发送消息成功！"<<endl;
            
            //接收对方发送过来的消息
            char recv_message[1024]="";
            
            //接收消息有误
            if(recv(fd,recv_message,1023,0)<=0)
            {
                cout<<"error!"<<endl;
            }
            else
            {
                cout<<"message from "<<ID<<" :"<<recv_message<<endl;
            }
        }
    }
    if(strncmp(recvbuff,"ok",2)==0)
    {
        cout<<"Connect to <<ID<<failed!"<<endl;
    }

}

void Login_success(int fd)//当服务器端反馈登录成功时，调用此函数进行处理
{
    while(1)
    {
        cout<<"请选择要进行的操作："<<endl;
        cout<<"1：获取在线用户列表"<<endl;
        cout<<"2：一对一聊天"<<endl;
        cout<<"3：群聊"<<endl;
        cout<<"4：下线并退出"<<endl;

        int  choice;
        cin>>choice;

        switch(choice)
        {
        case 1://获取在线用户列表
        {
            TYPE=TYPE_LIST;
            cout<<"TYPE:"<<TYPE<<endl;

            Json::Value val;
            val["type"]=TYPE;

            send(fd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
            
            cout<<"等待接收服务器数据..."<<endl;
            char buff[1024*1024]="";
            if(recv(fd,buff,1024*1024,0)<=0)
            {
                cout<<"error!"<<endl;
            }
            else
            {
                printf("客户端收到的数据为：%s\n",buff);
                fflush(stdout);
            }

            cout<<"在线的用户有："<<endl;
            int i=0;
            while(buff[i]!='\0')
            {
                if(buff[i]=='$')
                {
                    cout<<"\t";
                }
                else if(buff[i]=='#')
                {
                    cout<<endl;
                }
                else
                {
                    cout<<buff[i];
                }
                ++i;
            }
        }break;
        case 2://一对一聊天
        {
#if 0
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
#endif
            ChatToOne(fd);
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
        case 4://下线并退出
        {
            GoAway(fd);
            exit(0);
        }break;
    }
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

    cout<<"连接服务器成功！"<<endl;
    run(sockfd);
    close(sockfd);
}

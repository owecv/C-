#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

//MySQL头文件
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h> //包含MySQL所需要的头文件
#include <iostream>
#include <typeinfo>
#include <string.h>

//Json头文件
#include <json/json.h>

//多线程处理的头文件
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

char *ip="192.168.126.128";
unsigned short port=6000;

//MySQL
using namespace std;
#pragma comment(lib, "libmysql.lib")

//Json
using namespace std;
using namespace Json;

void function0(MYSQL mysql);//查询数据库的全部信息
void function1(MYSQL mysql);//查询数据库的第一列
void function2(MYSQL mysql);//一些API函数的测试
void function3(MYSQL mysql);//查询指定信息的用户信息
void function4(MYSQL mysql);//测试删除数据库中的记录
void function5();//在数据库中插入记录

int create_socket(char *ip,unsigned short port);//创建socket套接字

void user_login(MYSQL mysql,char *buff,int fd);//处理客户端的登录请求
void user_register(MYSQL mysql,char *buff,int fd);//处理客户端的用户注册请求
void user_exit(int fd);//处理客户端用户退出请求
void user_goaway(MYSQL mysql,char *buff,int fd);//处理客户端的下线请求
void user_dispaly(MYSQL mysql,int fd);//查询出所有在线用户，并发送给客户端
void user_chat_one_to_one(MYSQL mysql,char *buff,int fd);//处理客户端“一对一”聊天请求

void user_chat_one_to_one_success(void *arg);//当B同意和A进行“一对一聊天”，连接成功时调用此函数进行处理

void *user_process(void * arg);//服务器一直循环等待接收客户端的链接，如果接收到客户端的链接就把链接交给这个函数处理。然后继续循环等>     待接收其他客户端的链接

int main()
{
    int sockfd=create_socket(ip,port);
    struct sockaddr_in caddr;

    MYSQL mysql; //声明MySQL的句柄
    const char * host = "127.0.0.1";//因为是作为本机测试，所以填写的是本>    地IP
    const char * user = "root";//这里改为你的用户名，即连接MySQL的用户名
    const char * passwd = "891256";//这里改为你的用户密码
    const char * db = "chat";//这里改为你要连接的数据库的名字,一个数据可>    能有几张表
    unsigned int port = 3306;//这是MySQL的服务器的端口，如果你没有修改过>    的话就是3306。
    const char * unix_socket = NULL;//unix_socket这是unix下的，我在Window    s下，所以就把它设置为NULL
    unsigned long client_flag = 0;//这个参数一般为0

    mysql_init(&mysql);//连接之前必须使用这个函数来初始化

    MYSQL* sock = mysql_real_connect(&mysql, host, user, passwd, db, port    , unix_socket, client_flag);
    if (sock == NULL) //连接MySQL 
    {
        printf("Failed to connect to MySQL!\n");
        fprintf(stderr, " %s\n", mysql_error(&mysql));
        exit(1);
    }
    else
    {
        fprintf(stderr, "Connected to MySQL successfully!\n");
    }

    //用结构体封装要传递给process函数的参数
    typedef struct data
    {
        MYSQL mysql;
        int fd;
    }data;

    while(1)
    {
        cout<<"等待客户端连接..."<<endl;
        int len=sizeof(caddr);

        int c=accept(sockfd,(struct sockaddr*)&caddr,(socklen_t*)&len);//接收客户端链接
        
        printf("accept c=%d,ip=%s,port:%d\n",c,inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));

        pthread_t id;

        if(c<0)
        {
            continue;
        }
        else//如果服务器接收到来自客户端的连接，就创建一个线程，调用线程函数user_process()去处理它
        {
            //将要传递给porcess函数的参数封装到parm结构体中
            data parm;
            parm.mysql=mysql;
            parm.fd=c;

            pthread_create(&id,NULL,user_process,(void *)&parm);
        }
        //pthread_join(id,NULL);
    }
    //关闭MySQL连接
    mysql_close(&mysql);
    return 0;
}

int create_socket(char *ip,unsigned short port)
{   
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd!=-1);

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(port);
    saddr.sin_addr.s_addr=inet_addr(ip);

    int bin=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(bin!=-1);

    listen(sockfd,5);
    return sockfd;
}

//处理客户端的登录事件
void user_login(MYSQL mysql,char *buff,int fd)
{
    //cout<<"buff "<<buff;

    Json::Value val;
    Json::Reader read;

    //Json包解析失败
    if(-1==read.parse(buff,val))
    {
        cout<<"Json parse fail!"<<endl;
        return ;
    }

    MYSQL_RES * result;//保存结果集的

    if (mysql_set_character_set(&mysql, "gbk")) {   //将字符编码改为gbk
    fprintf(stderr, "错误,字符集更改失败！ %s\n", mysql_error(&mysql));
    }

    //拼接SQL语句
    string query = "select * from user where name='";
    string user_name=val["name"].asString().c_str();
    string temp="' and passwd='";
    query=query+user_name+temp;
    string user_passwd=val["passwd"].asString().c_str();
    query=query+user_passwd;
    temp.clear();
    temp="'";
    query=query+temp;
    cout<<"拼接完成的SQL查询语句为：" << query << endl;

    const char * i_query = query.c_str();

    if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
    {
        fprintf(stderr, "fail to query!\n");
        //打印错误原因
        fprintf(stderr, " %s\n", mysql_error(&mysql));
        
        cout<<"query error!"<<endl<<"不存在此用户！"<<endl;
        send(fd,"ok",2,0);

        return;
    }
    else
    {
        result=mysql_store_result(&mysql);
        int row_num = mysql_num_rows(result);//返回结果集中的行的数目
        cout<<"结果集中的记录行数为："<<row_num<<endl;
        if (row_num==0) //保存查询的结果
        {
            fprintf(stderr, "fail to store result!\n");
            cout<<"query error!"<<endl<<"不存在此用户！登录失败。。"<<endl;
            send(fd,"ok",2,0);
            return ;
        }
        else
        {
            MYSQL_ROW row;//代表的是结果集中的一行

            while ((row = mysql_fetch_row(result)) != NULL)
            //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
            {
                cout<<"query succeed!"<<endl<<"服务器端该用户的信息为："<<endl;
                
                printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                printf("用户状态： %s\n", row[3]);//打印当前行的第一列的数据
                fflush(stdout);
            }

            //数据库查询，存在此用户且密码正确，更改服务器端用户在线状态
            //拼接SQL语句
            string query = "update user set status='on' where name='";
            string user_name=val["name"].asString().c_str();
            string temp="'";
            query=query+user_name+temp;
            cout<<"拼接完成的SQL查询语句为：" << query << endl;

            const char * i_query = query.c_str();
            if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
            {
                fprintf(stderr, "fail to query!\n");
                //打印错误原因
                fprintf(stderr, " %s\n", mysql_error(&mysql));

                cout<<"query error!"<<endl;
                cout<<"服务器更新用户在线状态失败！"<<endl;
                send(fd,"ok",2,0);

                return;
            }
            else
            {
                //该用户上线成功，将该用户的sock_fd添加到MySQL数据库中
                char cmd[100]="";
                sprintf(cmd,"update user set sock_fd=%d where name='%s'",fd,val["name"].asString().c_str());

                cout<<"在MySQL中插入该用户和服务器链接的sock_fd的SQL语句为："<<endl;
                cout<<cmd<<endl;

                if (mysql_real_query(&mysql, cmd,strlen(cmd)) != 0)//如果连接成功，则开始查询，成功返回0
                {
                    fprintf(stderr, "fail to query!\n");
                    //打印错误原因
                    fprintf(stderr, " %s\n", mysql_error(&mysql));

                    cout<<"query error!"<<endl;
                    cout<<"服务器插入sock_fd失败！"<<endl;

                    return;
                }
                else
                {
                    cout<<"在MySQL中插入该用户与服务器连接的sock_fd成功！"<<endl;

                    //给客户点进行确认登录并反馈登录结果
                    send(fd,"OK",2,0);
                    cout<<"该用户上线成功！等待下一步操作..."<<endl;
                }
            }
        }
    }
    mysql_free_result(result);//释放结果集result
}

void user_register(MYSQL mysql,char *buff,int fd)//处理客户端的用户注册请求
{
    Json::Value val;
    Json::Reader read;

    //Json包解析失败
    if(-1==read.parse(buff,val))
    {
        cout<<"Json parse fail!"<<endl;
        return ;
    }

    MYSQL_RES * result;//保存结果集的

    if (mysql_set_character_set(&mysql, "gbk")) {   //将字符编码改为gbk
    fprintf(stderr, "错误,字符集更改失败！ %s\n", mysql_error(&mysql));
    }
    
    //拼接SQL语句
    string query = "insert into user (name,passwd) values('";
    string user_name=val["name"].asString().c_str();
    string temp="',";
    query=query+user_name+temp;
    string user_passwd=val["passwd"].asString().c_str();
    query=query+user_passwd;
    temp.clear();
    temp=")";
    query=query+temp;
    cout<<"拼接完成的SQL查询语句为：" << query << endl;

    const char * i_query = query.c_str();

    /*插入，成功则返回0*/
    int flag = mysql_real_query(&mysql, i_query, (unsigned int)strlen(i_query));
    if (flag)
    {
        printf("Insert data failure!\n");
        //打印错误原因
        fprintf(stderr, " %s\n", mysql_error(&mysql));
        //注册失败，并给客户端进行反馈
        send(fd,"ok",2,0);
    }
    else {
        printf("Insert data success!\n");
        //数据库插入新用户信息，并给客户反馈注册成功的结果
        send(fd,"OK",2,0);
        cout<<"该用户注册成功！等待下一步操作（登录或退出）"<<endl;
    }
}

void user_exit(int fd)//处理客户端用户退出请求
{
    //服务器处理客户端下线请求
    send(fd,"OK",2,0);

    //关闭socket套接字
    close(fd);
}

void user_goaway(MYSQL mysql,char *buff,int fd)//处理客户端的下线请求
{
    cout<<"服务器正在进行用户下线操作...";

    Json::Value val;
    Json::Reader read;

    //Json包解析失败
    if(-1==read.parse(buff,val))
    {
        cout<<"Json parse fail!"<<endl;
        return ;
    }

    MYSQL_RES * result;//保存结果集的

    if (mysql_set_character_set(&mysql, "gbk")) {   //将字符编码改为gbk
    fprintf(stderr, "错误,字符集更改失败！ %s\n", mysql_error(&mysql));
    }


    //拼接SQL语句
    string query = "update user set status='off' where name='";
    string user_name=val["name"].asString().c_str();
    string temp="'";
    query=query+user_name+temp;
    cout<<"拼接完成的SQL查询语句为：" << query << endl;

    const char * i_query = query.c_str();
    if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
    {
        fprintf(stderr, "fail to query!\n");
        //打印错误原因
        fprintf(stderr, " %s\n", mysql_error(&mysql));

        cout<<"query error!"<<endl;
        cout<<"服务器更新用户离线状态失败！"<<endl;
        send(fd,"ok",2,0);

        return;
    }
    else
    {
        //给客户点进行确认登录并反馈登录结果
        send(fd,"OK",2,0);
        cout<<"该用户下线成功！"<<endl;
    }

    //关闭socket套接字
    int c=fd;
    close(c);
}

void *user_process(void *arg)//服务器一直循环等待接收客户端的链接，如果接收到客户端的    链接就把链接交给这个函数处理。然后继续循环等待接收其他客户端的链接
{
    //将主函数传递给process函数的参数，进行解析
    typedef struct data
    {
        MYSQL mysql;
        int fd;//客户端的sock_fd
    }data;

    //将参数传进来
    data * parm=(data *)arg;
    MYSQL mysql=parm->mysql;
    int fd=parm->fd;

    cout<<"服务器端注册的所有用户，及各用户在线状态："<<endl;
    function0(mysql);//查询数据库的全部信息
    
    while(1)
    {
        char recv_buff[128]={0};

        void *info;

        if(recv(fd,recv_buff,127,0)<=0)
        {
            //关闭该socket连接
            close(fd);

            cout<<"error!"<<endl;
            cout<<"客户端意外退出！该连接已关闭！"<<endl;
        }
        else
        {
            printf("服务器(user_process)端收到的Json包为：\n%s",recv_buff);
            fflush(stdout);
        }

        //对接收缓冲区中的Json包进行解析
        Json::Value val;
        Json::Reader read;

        //Json包解析失败
        if(-1==read.parse(recv_buff,val))
        {
            cout<<"Json parse failed!"<<endl;
            return 0;
        }
        //判断客户端请求的操作类型
        if(val["type"]==0)//客户端请求登录
        {
            user_login(mysql,recv_buff,fd);
        }
        if(val["type"]==1)//客户端请求注册
        {
            user_register(mysql,recv_buff,fd);
        }
        if(val["type"]==2)//客户端请求退出
        {
            user_exit(fd);
            break;
        }
        if(val["type"]==3)//客户端请求下线并退出
        {
            user_goaway(mysql,recv_buff,fd);
            break;
        }
        if(val["type"]==4)//客户端请求列出所有在线用户
        {   
            user_dispaly(mysql,fd);//查询出所有在线用户，并发送给客户端
        }
        if(val["type"]==5)//客户端请求进行一对一聊天
        {
            info=user_chat_one_to_one(mysql,recv_buff,fd);
        }
        if(val["tppe"]==7)//客户端B同意和A进行“一对一聊天”
        {
            user_chat_one_to_one_success(info);
        }
    }
    //关闭socket套接字
    close(fd);
    return 0;
}

void user_dispaly(MYSQL mysql,int fd)//查询出所有在线用户，并发送给客户端
{
    MYSQL_RES * result;//保存结果集的

    if (mysql_set_character_set(&mysql, "gbk")) {   //将字符编码改为gbk
    fprintf(stderr, "错误,字符集更改失败！ %s\n", mysql_error(&mysql));
    }

    //拼接SQL语句
    string query = "select id,name from user where status='on'";

    cout<<"拼接完成的SQL查询语句为：" << query << endl;

    const char * i_query = query.c_str();

    if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询 .成功返回0
    {
        fprintf(stderr, "fail to query!\n");
        exit(1);
    }
    else
    {
        if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果
        {
            fprintf(stderr, "fail to store result!\n");
            exit(1);
        }
        else
        {
            MYSQL_ROW row;//代表的是结果集中的一行 
            //my_ulonglong row;
            int row_num = mysql_num_rows(result);//返回结果集中的行的数目
            cout<<"结果集中的记录行数为："<<row_num<<endl;

            cout<<"所有在线用户如下："<<endl;

            string user_online;//保存从结果集中分理出的在线用户信息

            while ((row = mysql_fetch_row(result)) != NULL)
            //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
            {
                printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                //printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                //printf("用户状态： %s\t", row[3]);//打印当前行的第一列的数据
                //fflush(stdout);
                cout << endl;
                string temp0="ID：";
                string temp1="$Name：";
                user_online=user_online+temp0+row[0]+temp1+row[1];
                string temp="#";
                user_online=user_online+temp;
            }
            cout<<"发送给客户端的数据为："<<user_online.c_str()<<endl;
            if(-1==send(fd,user_online.c_str(),strlen(user_online.c_str()),0))
            {
                cout<<"发送信息失败！"<<endl;
                return ;
            }   
        }
    }
    mysql_free_result(result);//释放结果集result
}

void *user_chat_one_to_one(MYSQL mysql,char *buff,int fd)//处理客户端“一对一”聊天请求
{
    cout<<"正在进行“一对一”聊天处理..."<<endl;

    Json::Value val;
    Json::Reader read;

    //Json包解析失败
    if(-1==read.parse(buff,val))
    {
        cout<<"Json parse fail!"<<endl;
        return ;
    }
   
    //一对一聊天时A的sock_fd
    int A_fd;
    //一对一聊天时B的sock_fd
    int B_fd;
    //一对一聊天时，A想要发送给B的消息内容
    char A_message_buff[1024]={0};

    typedef struct data
    {
        string A_name;//A的名字
        int A_fd;//A的fd
        string B_name;//B的名字
        int B_fd;//B的fd
        char A_message_buff[1024];
    }data;

    //查询客户请求“一对一聊天的对反是否在线”
    MYSQL_RES * result;//保存结果集的

    if (mysql_set_character_set(&mysql, "gbk")) {   //将字符编码改为gbk
    fprintf(stderr, "错误,字符集更改失败！ %s\n", mysql_error(&mysql));
    }

    //拼接SQL语句
    string query = "select * from user where name='";
    string user_name=val["name"].asString().c_str();
    string temp="' and status='on'";
    query=query+user_name+temp;
    
    cout<<"拼接完成的SQL查询语句为：" << query << endl;

    const char * i_query = query.c_str();

    if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
    {
        fprintf(stderr, "fail to query!\n");
        //打印错误原因
        fprintf(stderr, " %s\n", mysql_error(&mysql));

        cout<<"query error!"<<endl<<"不存在此用户！"<<endl;
        send(fd,"ok",2,0);

        return;
    }
    else
    {
        result=mysql_store_result(&mysql);
        int row_num = mysql_num_rows(result);//返回结果集中的行的数目
        cout<<"结果集中的记录行数为："<<row_num<<endl;
        if (row_num==0) //保存查询的结果
        {
            fprintf(stderr, "fail to store result!\n");
            cout<<"query error!"<<endl<<"该用户不在线！连接失败。。"<<endl;
            send(fd,"ok",2,0);
            return ;
        }
        else
        {
            MYSQL_ROW row;//代表的是结果集中的一行

            while ((row = mysql_fetch_row(result)) != NULL)
            //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
            {
                cout<<"query succeed!"<<endl<<"服务器端该用户的信息为："<<endl;

                printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                printf("用户状态： %s\t", row[3]);//打印当前行的第一列的数据
                printf("fd： %s\n", row[4]);//打印当前行的第一列的数据
                fflush(stdout);
             }
            send(fd,"OK",2,0);

            //拼接SQL语句，查询B的fd
            string query = "select * from user where name='";
            string user_name=val["B_name"].asString().c_str();
            string temp="'";
            query=query+user_name+temp;

            cout<<"拼接完成的SQL查询语句为：" << query << endl;

            const char * i_query = query.c_str();

            if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
            {
                fprintf(stderr, "fail to query!\n");
                //打印错误原因
                fprintf(stderr, " %s\n", mysql_error(&mysql));

                cout<<"query error!"<<endl<<"不存在此用户！"<<endl;
                send(fd,"ok",2,0);

                return;
            }
            else
            {
                result=mysql_store_result(&mysql);
                int row_num = mysql_num_rows(result);//返回结果集中的行的数目
                cout<<"结果集中的记录行数为："<<row_num<<endl;
                if (row_num==0) //保存查询的结果
                {
                    fprintf(stderr, "fail to store result!\n");
                    cout<<"query error!"<<endl<<"该用户不在线！连接失败。。"<<endl;
                    return ;
                }
                else
                {
                    //程序走到这一步，说明一对一聊天的双方都在线
                    MYSQL_ROW row;//代表的是结果集中的一行

                    while ((row = mysql_fetch_row(result)) != NULL)
                    //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
                    {
                        cout<<"query succeed!"<<endl<<"服务器端该用户的信息为："<<endl;

                        printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                        printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                        printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                        printf("用户状态： %s\t", row[3]);//打印当前行的第四列的数据
                        printf("sock_fd：%s\n",row[4]);//打印当前行的第五列的数据
                        B_fd=atoi(row[4]);
                        fflush(stdout);
                    }
                    printf("B的fd为：%d\n",B_fd);

                    fd_set fdset;
                    struct timeval timeout={5,0};

                    //检测A、B两个连接，那个连接上有发送数据的请求就处理那个连接，不会发生阻塞
                    while(1)
                    {
                        FD_ZERO(&fdset);
                        FD_SET(fd,&fdset);
                        FD_SET(B_fd,&fdset);

                        int n=select(B_fd+3,&fdset,NULL,NULL,&timeout);

                        if(n==-1)
                        {
                            cout<<"selection failure"<<endl;
                            break;
                        }
                        else if(n==0)
                        {
                            
                        }
                        if(FD_ISSET(fd,&fdset))//A发送的数据
                        {
                            if(recv(fd,A_message_buff,127,0)<=0)
                            {
                                cout<<"error!"<<endl;
                            }
                            else
                            {
                                cout<<"服务器收到的需要转发给B的信息为："<<endl;
                                cout<<A_message_buff<<endl;

                                //A请求和B进行“一对一聊天”
                                //服务器需要和B进行确认
                                char buff[20]="A_ask_B";//A向B请求一对一聊天

                                //服务器向B发送请求聊天的请求
                                if(-1==send(B_fd,buff,strlen(buff),0))
                                {   
                                    cout<<"发送信息“A_ask_B”失败！"<<endl;
                                    return ;
                                }
                                else
                                {   
                                    cout<<"给B发送“A_ask_B”成功！等待B的反馈..."<<endl;

                                //拼接SQL语句，查询A的fd
                                string query = "select * from user where name='";
                                string user_name=val["name"].asString().c_str();
                                string temp="'";
                                query=query+user_name+temp;

                                cout<<"拼接完成的SQL查询语句为：" << query << endl;

                                const char * i_query = query.c_str();
                                
                                if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询，成功返回0
                                {
                                    fprintf(stderr, "fail to query!\n");
                                    //打印错误原因
                                    fprintf(stderr, " %s\n", mysql_error(&mysql));

                                    cout<<"query error!"<<endl<<"不存在此用户！"<<endl;
                                    send(fd,"ok",2,0);

                                    return;
                                }
                                else
                                {
                                    result=mysql_store_result(&mysql);
                                    int row_num = mysql_num_rows(result);//返回结果集中的行的数目
                                    cout<<"结果集中的记录行数为："<<row_num<<endl;
                                    if (row_num==0) //保存查询的结果
                                    {
                                        fprintf(stderr, "fail to store result!\n");
                                        cout<<"query error!"<<endl<<"该用户不在线！连接失败。。"<<endl;
                                        return ;
                                    }
                                    else
                                    {
                                        //程序走到这一步，说明一对一聊天的双方都在线
                                        MYSQL_ROW row;//代表的是结果集中的一行

                                        while ((row = mysql_fetch_row(result)) != NULL)
                                        //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
                                        {
                                            cout<<"query succeed!"<<endl<<"服务器端该用户的信息为："<<endl;

                                            printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                                            printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                                            printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                                            printf("用户状态： %s\t", row[3]);//打印当前行的第四列的数据
                                            printf("sock_fd：%s\n",row[4]);//打印当前行的第五列的数据
                                            A_fd=atoi(row[4]);
                                            fflush(stdout);
                                        }
                                        printf("A的fd为：%d\n",A_fd);
                                    }
                                }

                                data parm;

                                parm.A_name=val["name"].asString().c_str();//A的名字
                                parm.A_fd=A_fd;//B的fd
                                parm.B_name=val["B_name"].asString().c_str();//B的名字
                                parm.B_fd=B_fd;//B的fd
                                parm.A_message_buff=A_message_buff;//A要发送给B的消息

                                return &parm;
#if 0        
                                    //对从客户端返回的数据进行处理
                                    //返回“YES”表示此用户同意与A进行“一对一聊天”
                                    char recvBbuff[10]="";

                                    //接收到的数据有误
                                    if(recv(B_fd,recvBbuff,9,0)<=0)
                                    {
                                        cout<<"error!"<<endl;
                                    }

                                    cout<<"一对一时，B给的反馈结果："<<endl<<recvBbuff<<endl;
                                    fflush(stdout);

                                    //对返回的数据进行判断
                                    if(strncmp(recvBbuff,"YES",3)==0)
                                    {
                                        cout<<"B同意进行“一对一聊天，准备转发A的消息...‘"<<endl;

                                        if(-1==send(B_fd,message_buff,strlen(message_buff),0))
                                        {
                                            cout<<"发送信息失败！"<<endl;
                                            return ;
                                        }
                                        else
                                        {
                                            cout<<"发送成功！"<<endl;
                                        }
                                    }
                                    if(strncmp(recvBbuff,"NO",2)==0)
                                    {
                                        cout<<"error!"<<endl;
                                        cout<<"B拒绝与A进行“一对一聊天!"<<endl;
                                    }
#endif
                                }
                            }
                        }
                        if(FD_ISSET(B_fd,&fdset))//B发送的数据
                        {
                            //cout<<"分析错误原因：此处不能用select！";
                            char message_buff[1024]={0};

                            if(recv(B_fd,message_buff,127,0)<=0)
                            {  
                                cout<<"error!"<<endl;
                            }   
                            else
                            {   
                                cout<<"服务器收到的需要转发给A的信息为："<<endl;
                                cout<<message_buff<<endl;

                                if(-1==send(fd,message_buff,strlen(message_buff),0))
                                {   
                                    cout<<"发送信息失败！"<<endl;
                                    return ;
                                }   
                                else
                                {   
                                    cout<<"发送成功！"<<endl;
                                }   
                            }
                        }
                    }
                }
            }
        }
    }
}

void user_chat_one_to_one_success(void *arg)//当B同意和A进行“一对一聊天”，连接成功时调用此函数进行处理
{
    typedef struct data
    {
        string A_name;//A的名字
        int A_fd;//A的fd
        string B_name;//B的名字
        int B_fd;//B的fd
        char A_message_buff[1024];
    }data;

    //将参数传进来
    data * parm=(data *)arg;
    string A_name=parm->A_name;
    int A_fd=parm->A_fd;
    string B_name=B_name;
    int B_fd=parm->B_fd;
    char A_message_buff[1024]=parm->A_message_buff[1024];

    if(-1==send(B_fd,A_message_buff,strlen(A_message_buff),0)    )   
    {
        cout<<"服务器转发A发送给B的消息失败！"<<endl;
        return ;
    }   
    else
    {
        cout<<"服务器转发A发送给B的消息成功！"<<endl;
    }
}

void function0(MYSQL mysql)//查询数据库的全部信息
{
	MYSQL_RES * result;//保存结果集的

	if (mysql_set_character_set(&mysql, "gbk")) {	//将字符编码改为gbk
		fprintf(stderr, "错误, %s\n", mysql_error(&mysql));
	}

	const char * i_query = "select * from user";//查询语句，从那个表中查询,这里后面没有;
	if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询 .成功返回0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			MYSQL_ROW row;//代表的是结果集中的一行 
						  //my_ulonglong row;
			while ((row = mysql_fetch_row(result)) != NULL)
				//读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
			{
                printf("ID： %s\t", row[0]);//打印当前行的第一列的数据        
                printf("姓名： %s\t", row[1]);//打印当前行的第二列的数据
                printf("密码： %s\t", row[2]);//打印当前行的第三列的数据
                printf("用户状态： %s\t", row[3]);//打印当前行的第一列的数据
                fflush(stdout);

                cout << endl;
			}
		}
	}
	mysql_free_result(result);//释放结果集result
}

void function1(MYSQL mysql)//查询数据库的第一列
{
	MYSQL_RES * result;//保存结果集的
	if (mysql_set_character_set(&mysql, "gbk")) {	//将字符编码改为gbk
		fprintf(stderr, "错误, %s\n", mysql_error(&mysql));
	}

	const char * i_query0 = "select 姓名 from info";//查询语句，从那个表中查询,这里后面没有;

	if (mysql_query(&mysql, i_query0) != 0)//如果连接成功，则开始查询 .成功返回0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			MYSQL_ROW row;//代表的是结果集中的一行
			while ((row = mysql_fetch_row(result)) != NULL)
			//读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
			{
				printf("姓名： %s\n", row[0]);//打印当前行的第一列的数据
			}
		}
	}
	mysql_free_result(result);//释放结果集result
}

void function2(MYSQL mysql)//一些API函数的测试
{
	MYSQL_RES * result;//保存结果集的
	if (mysql_set_character_set(&mysql, "gbk")) {	//将字符编码改为gbk
		fprintf(stderr, "错误, %s\n", mysql_error(&mysql));
	}

	const char * i_query = "select * from info";//查询语句，从那个表中查询,这里后面没有;
	if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询 .成功返回0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			int num = mysql_num_fields(result);//计算结果集中的列数
			int row_num = mysql_num_rows(result);//返回结果集中的行的数目
			cout << "/******************/" << endl;
			cout << "结果集中的列数为：" << num << endl;
			cout << "结果集中的行数为：" << row_num << endl;
			/*
			typedef struct st_mysql_res
			{
			my_ulonglong row_count;	                     // 结果集的行数
			unsigned int	field_count, current_field;	     // 结果集的列数，当前列
			MYSQL_FIELD	*fields;	                     // 结果集的列信息
			MYSQL_DATA	*data;	                         // 结果集的数据
			MYSQL_ROWS	*data_cursor;	                 // 结果集的光标
			MEM_ROOT	field_alloc;                         // 内存结构
			MYSQL_ROW	row;	                             // 非缓冲的时候用到
			MYSQL_ROW	current_row;                         // mysql_store_result时会用到。当前行
			unsigned long *lengths;	                     // 每列的长度
			MYSQL	*handle;	                             // mysql_use_result会用。
			my_bool	eof;	                             // 是否为行尾
			} MYSQL_RES;
			*/
			cout << "/*******************/" << endl;
			cout << "结构体指针的方法" << endl;
			cout << "结果集中的列数：" << result->field_count << endl;
			cout << "结果集中的行数：" << result->row_count << endl;
			cout << "当前行数：" << result->current_field << endl;

			cout << "/*******************/" << endl;
			cout << "返回上次UPDATE、DELETE或INSERT查询更改／删除／插入的行数" << endl;
			cout << mysql_affected_rows(&mysql) << endl;
			cout << "返回下一个表字段的类型" << endl;
			cout << mysql_fetch_field(result) << endl;
			cout << "返回全部字段结构的数组。" << endl;
			cout << mysql_fetch_fields(result) << endl;
			cout << "以字符串形式返回client版本号信息。" << endl;
			cout << mysql_get_client_info() << endl;
			cout << "以整数形式返回client版本号信息。" << endl;
			cout << mysql_get_client_version() << endl;
			cout << "返回描写叙述连接的字符串。" << endl;
			cout << mysql_get_host_info(&mysql) << endl;
			cout << "获取或初始化MYSQL结构。" << endl;
			cout << mysql_init(&mysql) << endl;
			cout << "检查与server的连接是否工作，如有必要又一次连接。" << endl;
			//返回1表示server的连接正常工作
			cout << mysql_ping(&mysql) << endl;
			cout << "选择数据库。" << endl;
			cout << mysql_select_db(&mysql, "test0") << endl;
			cout << "以字符串形式返回server状态。" << endl;
			cout << mysql_stat(&mysql) << endl;
			cout << "返回当前线程ID。" << endl;
			cout << mysql_thread_id(&mysql) << endl;
			cout << "初始化MySQL库" << endl;
			mysql_library_init;
			cout << "结束MySQL库的使用。" << endl;
			mysql_library_end();
			cout << "对于每一个非SELECT查询（比如INSERT、UPDATE、DELETE）。通过调用mysql_affected_rows()。可发现有多少行已被改变（影响）。" << endl;
			cout << mysql_affected_rows(&mysql) << endl;
}
	}
	mysql_free_result(result);//释放结果集result
}

void function3(MYSQL mysql)//查询指定信息的用户信息
{
	MYSQL_RES * result;//保存结果集的

	if (mysql_set_character_set(&mysql, "gbk")) {	//将字符编码改为gbk
		fprintf(stderr, "错误, %s\n", mysql_error(&mysql));
	}

	/*cout << "请输入用户名：";
	char *name ;
	cin >> *name;*/

	//const char * i_query = "select * from info where 性别='女'";//查询语句，从那个表中查询,这里后面没有;
	while (1) {
		string query = "select * from info where 姓名='";
		cout << "请输入用户名：";
		string name;
		cin >> name;
		query = query + name;
		string temp = "'";
		query = query + temp;
		cout << query << endl;
		const char * i_query = query.c_str();
		if (mysql_query(&mysql, i_query) != 0)//如果连接成功，则开始查询 .成功返回0
		{
			fprintf(stderr, "fail to query!\n");
			//打印错误原因
			fprintf(stderr, " %s\n", mysql_error(&mysql));
			exit(1);
		}
		else
		{
			if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果
			{
				fprintf(stderr, "fail to store result!\n");
				exit(1);
			}
			else
			{
				MYSQL_ROW row;//代表的是结果集中的一行 
							  //my_ulonglong row;
				int n = 1;
				while ((row = mysql_fetch_row(result)) != NULL)
					//读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
				{
					cout << "序号：" << n++ << "  ";
					//cout << mysql_fetch_field(result) << endl;
					printf("姓名： %s\t", row[0]);//打印当前行的第一列的数据
											   //cout <<"name is " << (char *)row[0] << "\t";
											   //cout << typeid(row[0]).name() << endl;
					printf("性别： %s\t", row[1]);//打印当前行的第二列的数据
											   //row = mysql_num_row(result);
											   //printf("%lu\n", mysql_num_row(result));
					printf("准考证号： %s\t", row[2]);//打印当前行的第一列的数据
					printf("证件编号： %s\t", row[3]);//打印当前行的第二列的数据
												 //cout << typeid(row[4]).name() << endl;
					printf("民族： %s\t\n", row[4]);//打印当前行的第一列的数据 
												 /*cout << row[0] << "\t";
												 cout << row[1] << "\t";*/
					cout << endl;
				}
			}
		}
	}
	mysql_free_result(result);//释放结果集result
}

void function4(MYSQL mysql)//删除数据库中的记录
{
	const char * query = "delete from info where 姓名='王子裕'";
	/*删除，成功则返回0*/
	int flag = mysql_real_query(&mysql, query, (unsigned int)
		strlen(query));
	if (flag) {
		printf("Delete data failure!\n");
	}
	else {
		printf("Delete data success!\n");
	}
	mysql_close(&mysql);
}

void function5()//在数据库中插入记录
{
	MYSQL mysql; //声明MySQL的句柄
	const char * host = "127.0.0.1";//因为是作为本机测试，所以填写的是本地IP
	const char * user = "root";//这里改为你的用户名，即连接MySQL的用户名
	const char * passwd = "891256";//这里改为你的用户密码
	const char * db = "mydatabase";//这里改为你要连接的数据库的名字,一个数据可能有几张表
	unsigned int port = 3306;//这是MySQL的服务器的端口，如果你没有修改过的话就是3306。
	const char * unix_socket = NULL;//unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL
	unsigned long client_flag = 0;//这个参数一般为0

	mysql_init(&mysql);//连接之前必须使用这个函数来初始化

	MYSQL* sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag);
	if (sock == NULL) //连接MySQL 
	{
		printf("fail to connect mysql \n");
		fprintf(stderr, " %s\n", mysql_error(&mysql));
		exit(1);
	}
	else
	{
		fprintf(stderr, "connect ok!\n");
	}

	if (mysql_set_character_set(&mysql, "gbk")) {	//将字符编码改为gbk
		fprintf(stderr, "错误, %s\n", mysql_error(&mysql));
	}

	const char * query = "insert into info(姓名,证件编号) values('华成龙',101)";
	/*插入，成功则返回0*/
	int flag = mysql_real_query(&mysql, query, (unsigned int)
		strlen(query));
	if (flag) {
		printf("Insert data failure!\n");
		//打印错误原因
		fprintf(stderr, " %s\n", mysql_error(&mysql));
	}
	else {
		printf("Insert data success!\n");
	}
	mysql_close(&mysql);
}

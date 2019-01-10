#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//linux系统调用有关的头文件
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>//文件操作有关的头文件
#include <sys/wait.h>
#include <string>
#include <iostream>
#include <sys/sendfile.h>
#include <unistd.h>

using namespace std;

#define SER_PORT 8000
#define SER_IP "192.168.126.130"

int socket_create();
void *thread_fun(void *arg);

int main()
{
    cout<<"ftp_ser启动成功！"<<endl;
    chdir("/home/wangpeng/代码/qq/文件传输服务");
    cout<<"更改ftp_ser运行目录成功！"<<endl;
    int sockfd=socket_create();
    struct sockaddr_in caddr;
    while(1)
    {
        int len=sizeof(caddr);
        int c=accept(sockfd,(struct sockaddr*)&caddr,(socklen_t*)&len);
        if(c<0)
        {
            continue;
        }

        printf("accept c=%d,ip=%s,port=%d\n",c,inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));
        pthread_t id;
        pthread_create(&id,NULL,thread_fun,(void *)&c);
    }
}

void *thread_fun(void *arg)
{
    int c=*(int*)arg;
    
    while(1)
    {
        char buff[128]={0};
        buff[127]=0;

        int n=recv(c,buff,127,0);//n表示从buff中读取了几个数据
        if(n<=0)
        {
            printf("one client over\n");
            break;//当一个客户端连接断开时，n<=0，这个时候就没有必要一直循环等待接收客户端的消息了，直接break跳出while循环
        }
        else
        {
            printf("服务器端收到的信息：%s\n",buff);
            fflush(stdout);
        }
        
        char* ptr=NULL;//strtok_r会用到的一个参数
        char* s=strtok_r(buff," ",&ptr);
        
        if(NULL==s)//没有分割出命令
        {
            continue;
        }
        
        //将命令和参数分割出来
        char* myargv[10]={0};//这个数组里存了命令和参数
        myargv[0]=s;
       
        int i=1;
        while((s=strtok_r(NULL," ",&ptr))>0)
        {
            myargv[i++]=s;
        }

        if(strcmp(myargv[0],"ls")==0)
        {
            int pipefd[2];
            pipe(pipefd);//创建管道
        
            pid_t pid=fork();//先创建管道，再进行fork

            if(pid==-1)
            {
                printf("复制进程出错！\n");
            }
            if(pid==0)//0为子进程
            {
                dup2(pipefd[1],1);//1为读端
                dup2(pipefd[1],2);//2为写端
            
                printf("**************可以下载的文件有**************\n");
                execvp(myargv[0],myargv);
                perror("execvp error");
                exit(0);
            }
            
            close(pipefd[1]);
            wait(NULL);
            char readbuff[1024]={0};
            int pipe_num=read(pipefd[0],readbuff,1023);

            send(c,readbuff,pipe_num,0);
            close(pipefd[0]);
        }
        if(strcmp(myargv[0],"rm")==0)
        {
            char cmdl[64]="rm ";
            
            strcat(cmdl,myargv[1]);
            printf("cmdl=%s\n",cmdl);
            system(cmdl);
        }
        if(strcmp(myargv[0],"get")==0)
        {
            int fd=open(myargv[1],O_RDONLY);
            if(-1==fd)
            {
                printf("open file error!");
                exit(1);//如果代开文件失败，直接退出程序
            }
            else
            {
                cout<<"open file success!"<<endl;
            }

            int size=lseek(fd,0,SEEK_END);//计算要下载文件的大小
            lseek(fd,0,SEEK_SET);
            
            if(size==0)
            {
                cout<<"文件内容为0！"<<endl;
                continue;
            }
            else
            {
                cout<<"size:"<<size<<endl;
            }

            char file_size[10]={0};
            sprintf(file_size,"%d",size);// 把1234 传到了buffer 而buffer为char *
            fflush(stdout);

            cout<<"file_size:"<<file_size<<endl;

            send(c,file_size ,10,0);
            
            sendfile(c,fd,NULL,size);
        }
        if(strcmp(myargv[0],"put")==0)
        {
            char file_size[10]={0};
            if(recv(c,file_size,10,0)<=0)
            {
                return 0;
            }
            printf("要上传的文件大小为：%s\n",file_size);
            
            send(c,"OK",2,0);//服务器端收到文件的大小，并给客户端进行反馈确认
            
            int size=atoi(file_size);
            cout<<"要创建的文件名为："<<myargv[1]<<endl;
            int fd=open(myargv[1],O_WRONLY | O_CREAT,0600);
            if(fd==-1)
            {
                cout<<"在服务器端创建要接收的文件失败！"<<endl;
                return 0;
            }

            char data_size[512]={0};
            int num=0;

            int curr_size=0;//当前文件的大小
            while((num=recv(c,data_size,512,0))>0)
            {
                write(fd,data_size,num);//写入文件
                curr_size+=num;
                if(curr_size==size)//接收文件时，对文件的大小进行校验，当接收文件的大小等于文件大小时，说明文件已经接收完了，break退出循环即可
                {
                    cout<<"文件上传成功！"<<endl;
                    break;
                }
            }
        }
        if(strcmp(myargv[0],"pwd")==0)
        {
            int pipefd[2];
            pipe(pipefd);//创建管道
        
            pid_t pid=fork();//先创建管道，再进行fork

            if(pid==-1)
            {
                printf("复制进程出错！\n");
            }
            if(pid==0)//0为子进程
            {
                dup2(pipefd[1],1);//1为读端
                dup2(pipefd[1],2);//2为写端
            
                printf("**************ftp_ser运行目录**************\n");
                execvp(myargv[0],myargv);
                perror("execvp error");
                exit(0);
            }
            
            close(pipefd[1]);
            wait(NULL);
            char readbuff[1024]={0};
            int pipe_num=read(pipefd[0],readbuff,1023);

            send(c,readbuff,pipe_num,0);
            close(pipefd[0]);
        }
    }
    close(c);
}

int socket_create()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        return -1;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));

    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(SER_PORT);
    saddr.sin_addr.s_addr=inet_addr(SER_IP);

    int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res==-1)
    {
        return -1;
    }

    listen(sockfd,5);
    return sockfd;
}



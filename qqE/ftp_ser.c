#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//linux系统调用有关的头文件
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<fcntl.h>//文件操作有关的头文件

#define SER_PORT 8000
#define SER_IP "127.0.0.1"

int socket_create();
void *thread_fun(void *arg);

int main()
{
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
        pthread_create(&id,NULL,thread_fun,(void *)c);
    }
}

void *thread_fun(void *arg)
{
    int c=(int)arg;
    while(1)
    {
        char buff[128]={0};
        int n=recv(c,buff,127,0);//n表示从buff中读取了几个数据
      
        printf("客户端执行命令：%s\n",buff);
        if(n<=0)
        {
            break;
        }
       
        char* myargv[10]={0};
        char* ptr=NULL;
        char* s=strtok_r(buff," ",&ptr);
        
        if(NULL==s)
        {
            send(c,"ok#",3,0);
            continue;
        }
        
        int i=1;
        myargv[0]=s;
       
        while((s=strtok_r(NULL," ",&ptr))>0)
        {
            myargv[i++]=s;
            printf("%s\n",s);
        }

        if(strcmp(myargv[0],"get")==0)
        {
            int fd=open(myargv[1],O_RDONLY);
            if(-1==fd)
            {
                printf("open file error");
                send(c,"err",3,0);
                continue;
            }

            int size=lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_SET);

            char file_size[64]={"ok#"};
            sprintf(file_size+3,"%d",size);
            send(c,file_size,strlen(file_size),0);
            if(size==0)
            {
                continue;
            }

            char statu[32]={0};
            if(recv(c,statu,31,0)<=0)
            {
                break;
            }
            if(strcmp(statu,"ok")!=0)
            {
                continue;
            }
            printf("%s\n",statu);
            printf("%d\n",size);
            sendfile(c,fd,NULL,size);

        }
        else if(strcmp(myargv[0],"put")==0)
        {
            char buff[64]={0};
            if(recv(c,buff,63,0)<=0)
            {
                return;
            }
            printf("%s\n",buff);
            if(strncmp(buff,"ok#",3)!=0)
            {
                printf("ERROR:%s\n",buff+3);
                return;
            }

            int size=0;
            sscanf(buff+3,"%d",&size);
            printf("str size=%s,size=%d\n",buff+3,size);

            int fd=open(myargv[1],O_WRONLY | O_CREAT,0600);
            if(fd==-1)
            {
                send(c,"err",3,0);
                return;
            }
            send(c,"ok",2,0);

            char data_size[512]={0};
            int num=0;

            int curr_size=0;
            while((num=recv(c,data_size,512,0))>0)
            {
                write(fd,data_size,num);
                curr_size+=num;
                if(curr_size==size)
                {
                    break;
                }
            }
        }
        else if(strcmp(myargv[0],"ls")==0)
        {
            int pipefd[2];
            pipe(pipefd);//创建管道
            //printf("pipe\n");
            pid_t pid=fork();//先创建管道，再进行fork
            assert(pid!=-1);
            
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
            char readbuff[1024]={"ok#"};
            int pipe_num=read(pipefd[0],readbuff+3,1020);

            send(c,readbuff,pipe_num+3,0);
            close(pipefd[0]);

        }
        else 
        {
            int pipefd[2];
            pipe(pipefd);//创建管道
            //printf("pipe\n");
            pid_t pid=fork();//先创建管道，再进行fork
            assert(pid!=-1);

            if(pid==0)//0为子进程
            {
                dup2(pipefd[1],1);//1为读端
                dup2(pipefd[1],2);//2为写端
                
                execvp(myargv[0],myargv);
                perror("execvp error");
                exit(0);
            }

            close(pipefd[1]);
            wait(NULL);
            char readbuff[1024]={"ok#"};
            int pipe_num=read(pipefd[0],readbuff+3,1020);
            send(c,readbuff,pipe_num+3,0);
            close(pipefd[0]);
        }
    }
    close(c);
    printf("one client over\n");
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


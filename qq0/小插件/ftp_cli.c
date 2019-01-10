#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include <iostream>
using namespace std;

#define SER_PORT 8000
#define SER_IP "192.168.126.130"

//接收文件
void recv_file(int sockfd,char *name)
{
    char file_size[10]={0};
    if(recv(sockfd,file_size,10,0)<=0)
    {
        return;
    }
    
    int size=atoi(file_size);
    printf("要接收的文件信息大小=%d\n",size);
    
    cout<<"文件名为："<<name<<endl;

    int fd=open(name,O_WRONLY | O_CREAT,0600);
    if(fd==-1)
    {
        send(sockfd,"err",3,0);
        cout<<"在本地创建文件失败！"<<endl;
        return;
    }

    char data_size[512]={0};//每次接收512字节的文件内容
    
    int num=0;//记录成功接收到的文件数据字节数
    int curr_size=0;

    while((num=recv(sockfd,data_size,512,0))>0)
    {
        write(fd,data_size,num);
        curr_size+=num;

        float f=curr_size * 100/size;
        fflush(stdout); 
        printf("download:%2.f%%\r",f);
        if(curr_size==size)
        {
            break;
        }
    }
    printf("download successful!\n\n");
}

int main()
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
    
    int res=connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    assert(res!=-1);

    while(1)
    {
        char buff[128]={0};
        setbuf(stdin,NULL);//清空输入缓冲区
        printf("connet sccess>> ");
        fflush(stdout);
        fgets(buff,128,stdin);//从键盘读取命令
        buff[strlen(buff)-1]=0;
        
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        
        char cmd[128]={0};//存命令
        strcpy(cmd,buff);
        
        char* ptr=NULL;//strtok_r会用到的一个参数
        char* s=strtok_r(cmd," ",&ptr);

        if(s==NULL)
        {
            continue;
        }
        if(strcmp(s,"exit")==0)
        {
            break;
            exit(0);
        }
        if(strcmp(s,"ls")==0)
        {
            send(sockfd,buff,strlen(buff),0);
            cout<<"buff:"<<buff<<endl;

            char recvbuff[1024]={0};
            if(recv(sockfd,recvbuff,1023,0)<0)
            {
                continue;
            }
            else//打印出服务器端的ls结果
            {
                printf("%s\n",recvbuff);
            }
        }
        if(strcmp(s,"get")==0)
        {
            send(sockfd,buff,strlen(buff),0);
            
            cmd[128]={0};//存命令
            strcpy(cmd,buff);
            ptr=NULL;//strtok_r会用到的一个参数
            
            s=strtok_r(cmd," ",&ptr);
            if(s==NULL)
            {
                continue;
            }
            s=strtok_r(NULL," ",&ptr);//分割出文件名
            recv_file(sockfd,s);//接收文件
        }
        if(strcmp(s,"put")==0)
        {
            send(sockfd,buff,strlen(buff),0);
            
            cmd[128]={0};//存命令
            strcpy(cmd,buff);
            ptr=NULL;//strtok_r会用到的一个参数
            
            s=strtok_r(cmd," ",&ptr);//s为命令
            if(s==NULL)
            {
                continue;
            }
            s=strtok_r(NULL," ",&ptr);//分割出文件名
            
            int fd=open(s,O_RDONLY);//以只读方式打开需要上传的文件
            if(fd==-1)
            {
                printf("open file error！\n");
                continue;
            }
            else
            {
                cout<<"open file success!"<<endl;
            }
            
            int size=lseek(fd,0,SEEK_END);//计算需要上传的文件的大小
            if(size==0)
            {
                cout<<"size=0"<<endl;
                continue;
            }
            else
            {
                lseek(fd,0,SEEK_SET);//将文件指针回零
                cout<<"要上传的文件大小为："<<size<<endl;
            }
            
            char file_size[10]={0};
            sprintf(file_size,"%d",size);// 把1234 传到了buffer 而buffer为char *
            cout<<"file_size:"<<file_size<<endl;
            fflush(stdout);

            send(sockfd,file_size ,10,0);
            
            char recvbuff[20]={0};//记录服务器的反馈结果
            if(recv(sockfd,recvbuff,19,0)<0)
            {
                cout<<"未接收到服务器对要上传的文件大小信息的确认信息！"<<endl;
            }
            else//打印出服务器端的ls结果
            {
                cout<<"已收到服务器对要上传的文件大小信息的确认信息！"<<endl;
                printf("%s\n",recvbuff);
            }

            sendfile(sockfd,fd,NULL,size);//系统提供的函数
        }
        if(strcmp(s,"pwd")==0)
        {
            send(sockfd,buff,strlen(buff),0);
            cout<<"buff:"<<buff<<endl;

            char recvbuff[1024]={0};
            if(recv(sockfd,recvbuff,1023,0)<0)
            {
                continue;
            }
            else//打印出服务器端的ls结果
            {
                printf("%s\n",recvbuff);
            }
        }
        if(strcmp(s,"lsl")==0)
        {
            printf("*************本地文件可以操作的文件*********\n");
            system("ls");
            printf("********************************************\n");
        }
        //打印目前所处的路径（客户端）
        if(strcmp(s,"pwdl")==0)
        {
            char cmdl[64]="pwd";
            printf("本地路径为：");
            fflush(stdout);
            system(cmdl);
        }
    }
    close(sockfd);
    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<fcntl.h>
#define SER_PORT 6000
#define SER_IP "127.0.0.1"

void recv_file(int sockfd,char *name)
{
    char buff[64]={0};
    if(recv(sockfd,buff,63,0)<=0)
    {return;}
    if(strncmp(buff,"ok#",3)!=0)
    {
        printf("Error: %s\n",buff+3);
        return;
    }

    int size=0;
    sscanf(buff+3,"%d",&size);
    printf("要接收的文件信息大小=%s\n",buff+3/*,size*/);

    int fd=open(name,O_WRONLY | O_CREAT,0600);
    if(fd==-1)
    {
        send(sockfd,"err",3,0);
        return;
    }
    send(sockfd,"ok",2,0);

    char data_size[512]={0};
    int num=0;

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
        printf("connet sccess>> ");
        fflush(stdout);
        fgets(buff,128,stdin);
        buff[strlen(buff)-1]=0;
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        
        char cmd[128]={0};
        strcpy(cmd,buff);

        char *s=strtok(cmd," ");
        if(s==NULL)
        {
            continue;
        }
        if(strcmp(s,"exit")==0)
        {
            break;
            exit(0);
        }
        else if(strcmp(s,"get")==0)
        {
            s=strtok(NULL," ");
            if(s==NULL)
            {
                continue;
            }

            send(sockfd,buff,strlen(buff),0);
            recv_file(sockfd,s);

        }
        else if(strcmp(s,"put")==0)
        {
            s=strtok(NULL," ");
            if(s==NULL)
            {
                continue;
            }
            send(sockfd,buff,strlen(buff),0);
            int fd=open(s,O_RDONLY);
            if(fd==-1)
            {
                printf("open file error\n");
                send(sockfd,"err",3,0);
                continue;
            }
            int size=lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_SET);

            char file_size[64]={"ok#"};
            sprintf(file_size+3,"%d",size);
            printf("要上传的文件大小为：%d\n",size);
            send(sockfd,file_size,strlen(file_size),0);
            if(size==0)
            {
                continue;
            }

            char statu[32]={0};
            if(recv(sockfd,statu,31,0)<=0)
            {break;}
            if(strcmp(statu,"ok")!=0)
            {continue;}
            
            sendfile(sockfd,fd,NULL,size);
        }
        else if(strcmp(s,"lsl")==0)
        {
            printf("*************本地文件可以操作的文件*********\n");
            system("ls -l");
            printf("********************************************\n");
        }
        else if(strcmp(s,"mkdirl")==0)
        {
            char cmdl[64]="mkdir ";
            char* sl;
            sl=strtok(NULL," ");
            {
                if(sl==NULL)
                {
                    continue;
                }
            }

            strcat(cmdl,sl);
            printf("cmdl=%s\n",cmdl);
            system(cmdl);
        }
        else if(strcmp(s,"cdl")==0)
        {
            char cmdl[64]="cd ";
            char buffl[128];
            //char sl[64];
            int i=0;
            for(;i<128;++i)
            {
                buffl[i]=buff[i];
            }
            char* sl;
            sl=strtok(buffl," ");
            {
                if(sl==NULL)
                {
                    continue;
                }
            }
            char* sl1;
            sl1=strtok(NULL," ");
            {
                if(sl1==NULL)
                {
                    continue;
                }
            }
            strcat(cmdl,sl1);
            printf("cmdl=%s\n",cmdl);
            //system(cmdl);
#if 0
            pid_t pid=fork();
            if(pid==0)
            {   
                execl("/bin/cd",sl1,NULL);
            }
#endif   
        }
#if 1
        else if(strcmp(s,"rml")==0)
        {
            char cmdl[64]="rm ";
            char buffl[128];
            //char sl[64];
            int i=0;
            for(;i<128;++i)
            {
                buffl[i]=buff[i];
            }
            char* sl;
            sl=strtok(buffl," ");
            {
                if(sl==NULL)
                {
                    continue;
                }
            }
            char* sl1;
            sl1=strtok(NULL," ");
            {
                if(sl1==NULL)
                {
                    continue;
                }
            }
            strcat(cmdl,sl1);
            printf("cmdl=%s\n",cmdl);
            system(cmdl);
        }
#endif

#if 1
        //删除本地目录
        else if(strcmp(s,"rmldir")==0)
        {
            char cmdl[64]="rm -r ";
            char buffl[128];
            //char sl[64];
            int i=0;
            for(;i<128;++i)
            {
                buffl[i]=buff[i];
            }
            char* sl;
            sl=strtok(buffl," ");
            {
                if(sl==NULL)
                {
                    continue;
                }
            }
            char* sl1;
            sl1=strtok(NULL," ");
            {
                if(sl1==NULL)
                {
                    continue;
                }
            }
            strcat(cmdl,sl1);
            printf("cmdl=%s\n",cmdl);
            system(cmdl);
        }
#endif

        //打印目前所处的路径（客户端）
        else if(strcmp(s,"pwdl")==0)
        {
            char cmdl[64]="pwd";
            printf("本地路径为：");
            fflush(stdout);
            system(cmdl);
        }

        else
        {
            send(sockfd,buff,strlen(buff),0);
            char recvbuff[1024]={0};
            if(recv(sockfd,recvbuff,1023,0)<0)
            {continue;}

            if(strncmp(recvbuff,"ok#",3)==0)
            {
                printf("%s\n",recvbuff+3);
            }
            else
            {
                printf("recv error\n",recvbuff+3);
            }

        }

    }
    close(sockfd);
}


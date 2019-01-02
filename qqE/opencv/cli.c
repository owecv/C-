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

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#include <json/json.h>
using namespace Json;

#define IP "192.168.126.130"
#define PORT 7000

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

	//打开摄像头
	//从摄像头读入视频
	Mat image;
    VideoCapture capture(0);
    vector<uchar> data_encode;
    while(1)
    {
        //发送数据
		//循环显示每一帧
    	if (!capture.read(image)) 
            break;

		imshow("cli", image);
		if (waitKey(30) == 27) break;

        imencode(".jpg", image, data_encode);
        int len_encode = data_encode.size();
        string len = to_string(len_encode);
        int length = len.length();
        for (int i = 0; i < 16 - length; i++)
        {
            len = len + " ";
        }
        //发送数据
        send(sockfd, len.c_str(), strlen(len.c_str()), 0);
        char send_char[1];
        for (int i = 0; i < len_encode; i++)
        {
            send_char[0] = data_encode[i];
            send(sockfd, send_char, 1, 0);
        }
        //接收返回信息
        char recvBuf[32];
        if(recv(sockfd, recvBuf, 32, 0));
            //printf("%s\n", recvBuf);
	}
    close(sockfd);
	return 0;
}

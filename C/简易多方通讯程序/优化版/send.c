#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>
#include<bits/pthreadtypes.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <errno.h>
#include <dirent.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <time.h>
#include <linux/fb.h>
int client_fd;
/*
	init_tcp_client:tcp客户端的初始化
*/
int init_tcp_client(char*ip,char*port)
{
	//1.创建一个套接字  
	client_fd = socket(AF_INET,SOCK_STREAM,0);
	if(client_fd == -1)
	{
		perror("socket error:");
		return -1;
	}
	
	//设置网络地址结构体
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	//2.连接服务器
	int re = connect(client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(re == -1)
	{
		perror("connect error:");
		return -1;
	}
	printf("connect success\n");
	
	return 0;
}

/*
	communication:通信
*/

int flag=1;

void *func(void *data)
{
	while(1)
	{
		char buf[32]={0};
	
		int re=recv(client_fd,buf,32,0);
		if(re>0)	
		if(flag)printf("%s\n",buf);
		//send(sock,"ok",2,0);

	}
}
int main(int argc,char**argv)
{
	//tcp客户端的初始化
	init_tcp_client(argv[1],argv[2]);
	pthread_t pt;
	pthread_create(&pt,NULL,func,NULL);
	
	//通信
	while(1)
	{
		char buf[32] = {0};
		gets(buf);

		send(client_fd,buf,strlen(buf),0);
		if(strcmp(buf,"offline")==0)
		{
			flag=0;
		}
		if(strcmp(buf,"online")==0)
		{
			flag=1;
			
		}
		if(strcmp(buf,"#poweroff")==0)
		{
			break;
		}

	}
	//4.关闭网络连接
	close(client_fd);
}
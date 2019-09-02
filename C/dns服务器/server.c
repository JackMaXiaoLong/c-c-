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
/*
	init_udp_server:udp服务端的初始化
*/
int init_udp_server(char*ip,char*port)
{
	//创建套接字
	int server_fd = socket(AF_INET,SOCK_DGRAM,0);
	
	//设置服务器的网络地址结构体
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; 
	server_addr.sin_port = htons(atoi(port));
	inet_aton(ip,&server_addr.sin_addr);
	
	//2.绑定服务端的ip地址和端口号到socket上
	int re = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(re == -1)
	{
		perror("bind error:");
		return -1;
	}	
	return server_fd;
}
/*
	time_pro：时间协议，获取时间发送给客户端
*/
void time_pro(int server_fd,char*name)
{
	//服务端先接收数据  date
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	
	while(1)
	{
		FILE *fd=fopen(name, "r+");
		char buf[32] = {0};
		recvfrom(server_fd,buf,32,0,(struct sockaddr*)&client_addr,&len);
		
		
		
		while(1)
		{
			int i=0;char s[200]={0};
			char *p=fgets(s, 200, fd);
			if(p==NULL)
			{
				sendto(server_fd,"no",2,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
				break;
			}

			while(*p!=' '){p++;i++;}
			p++;
			if(strncmp(buf,p,strlen(buf)) == 0)
			{
				char buf1[32]={0};
				strncpy(buf1,s,i);
				printf("%s\n",buf1);
				sendto(server_fd,buf1,strlen(buf1),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
				//printf("hehe\n");
				break;
			}

		}
		//printf("haha\n");
		fclose(fd);
		
	}
	
}
int main(int argc,char**argv)
{
	//udp服务端的初始化
	int server_fd = init_udp_server(argv[1],argv[2]);
	
	//时间协议，获取时间发送给客户端
	time_pro(server_fd,argv[3]);
	
	//关闭网络连接
	close(server_fd);
}
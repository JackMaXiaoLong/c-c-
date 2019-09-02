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
#include <sys/types.h>         
#include <sys/socket.h>
#include <sys/socket.h>			   
#include <netinet/in.h>			   
#include <arpa/inet.h>
#include <linux/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "linklist.h"
int *p;
//并发接收
int connect_fd;
struct headnode* currenthead;


void *func1(void *data)
{
	struct node *p=currenthead->first;
	while(p!=NULL)
	{

		send(connect_fd,p->data,strlen(p->data),0);
		p=p->next;
		char buf[32];
		recv(connect_fd,buf,32,0);
	}
	send(connect_fd,"end",3,0);
}

void *func2(void *data)
{
	int n;
	int fd=open((char*)data,O_RDWR);
	while(1)
	{				
		char buf[512]={0};
		n = read(fd, buf, 512);	
		if(n == 0)
		{
			send(connect_fd,"connectend",10,0);
			break;
		}
		send(connect_fd,buf,n,0);
	}
}
void *func3(void *data)
{
	int fd=open((char *)data, O_RDWR|O_CREAT,0777);
	while(1)
	{
		char buf[512]={0};
		int r = recv(connect_fd,buf,512,0);
		char *p=buf;
		p=p+r-10;
		if (r > 0)
		{
			if(strcmp(p,"connectend")==0)
			{
				write(fd, buf, r-10);
				break;
			}
			else write(fd, buf, r);
		}
	}
	close(fd);
	
}


int main(int argc, char *argv[])
{
	//创建一个套接字
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("socket error:");
		return -1;
	}

	//绑定一个地址(ip+端口号)
	struct sockaddr_in sa;
	memset(&sa, 0,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));  
	sa.sin_addr.s_addr = inet_addr( argv[1]);
	int r = bind(sock,  (struct sockaddr*)&sa, sizeof(sa));
	if (r == -1)
	{
		perror("bind error:");
		return -1;
	}

	//监听
	listen(sock, 10);

	chdir("/mnt/hgfs/Program/1");
	
	//等待客户端的连接请求
	while (1)
	{
		char buf[512] ;
		struct sockaddr_in sa1;
		socklen_t addrlen = sizeof(sa1); 
		connect_fd = accept(sock, (struct sockaddr *)&sa1, &addrlen);
		if (connect_fd == -1)
		{
			perror("accept error");
			continue;
		}
		printf("receive from %s [port :%d]\n", inet_ntoa(sa1.sin_addr),ntohs(sa1.sin_port));
		pid_t pid = fork();
		if (pid > 0)
		{
			//父进程继续监听
			close(connect_fd);
			continue; 		
		}
		else if (pid == 0)
		{
			close(sock);
			currenthead=create_head();
			pthread_t pt;
			while(1)
			{
				qingkong(currenthead);
				find_file("/mnt/hgfs/Program/1",currenthead);
				struct node *p=currenthead->first;

				char buf[512]={0};
				recv(connect_fd,buf,512,0);
				if(strcmp(buf,"ls")==0)	
				{
					pthread_create(&pt,NULL,func1,NULL);
				}
				if(strncmp(buf,"get",3)==0)
				{
					char *p=buf;p+=4;
					pthread_create(&pt,NULL,func2,(void*)p);
				}
				if(strncmp(buf,"put",3)==0)
				{
					char *p=buf;p+=4;
					pthread_create(&pt,NULL,func3,(void*)p);
				}
				pthread_join(pt,NULL);
			}
		}
	}
}

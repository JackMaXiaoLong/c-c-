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
//并发接收
int flag=1;
int fd;
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
		int connect_fd = accept(sock, (struct sockaddr *)&sa1, &addrlen);
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
			while(1)
			{
				memset(buf,0,512);
				if(flag)
				{
					r = recv(connect_fd,buf,512,0);
					if (r > 0)
					{
						fd=open(buf, O_RDWR|O_CREAT,0777);
						flag=0;
					}
				}
				r = recv(connect_fd,buf,512,0);
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
			exit(0);
		}
	}
}

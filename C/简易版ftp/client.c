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
//发送一个文件给服务器，文件名由命令行传入
int main(int argc, char *argv[])
{

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("socket error:");
		return -1;
	}
	struct sockaddr_in sa;
	memset(&sa, 0,sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons( atoi(argv[2])); 
	sa.sin_addr.s_addr = inet_addr( argv[1]);

	int r = connect(sock, (struct sockaddr*)&sa, sizeof(sa));
	if (r == -1)
	{
		perror("connect error");
		return -1;
	}
	send(sock,argv[3],strlen(argv[3]),0);
	sleep(1);
	int n;
	int fd=open(argv[3],O_RDWR);
	while(1)
	{				
		char buf[512]={0};
		n = read(fd, buf, 512);	
		if(n == 0)
		{
			send(sock,"connectend",10,0);
			break;
		}
		send(sock,buf,n,0);
	}
	close(sock);
}
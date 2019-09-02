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


int main(int srgc,char**argv)
{
	//1.创建套接字
	int client_fd = socket(AF_INET,SOCK_DGRAM,0);
		
	//设置服务器的网络地址结构体
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; 
	server_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1],&server_addr.sin_addr);
		
	//2.通信
	socklen_t len = sizeof(server_addr);
	while(1)
	{
		char buf[32] = {0};
		gets(buf);
		sendto(client_fd,buf,strlen(buf),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
		
		char timebuf[128] = {0};
		int re = recvfrom(client_fd,timebuf,32,0,(struct sockaddr*)&server_addr,&len);
		if(re <= 0)
		{
			continue;
		}
		printf("ip = %s\n",timebuf);
		
		//char buf1[128];
		//sprintf(buf1,"date -s '%s'",timebuf);
		
		//system(buf1);
		
		//execlp("date","date","-s",timebuf,NULL);
		//break;
		
	}
		
		
	//3.关闭网络连接
	close(client_fd);
}
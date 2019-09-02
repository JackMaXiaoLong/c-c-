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

int sock;


void *func1(void *data)
{
	printf("******\n" );
	while(1)
	{
		char buf[32]={0};
	
		recv(sock,buf,32,0);
		if(strcmp(buf,"end")==0){
			printf("******\n" );
			break;}
		printf("%s\n",buf);
		send(sock,"ok",2,0);

	}

}
void *func2(void *data)
{
	int fd=open((char *)data, O_RDWR|O_CREAT,0777);
	while(1)
	{
		char buf[512]={0};
		int r = recv(sock,buf,512,0);
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
	printf("ok\n");
}
void *func3(void *data)
{
	int n;
	int fd=open((char*)data,O_RDWR);
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
	printf("ok\n");
}


int main(int argc, char *argv[])
{
	chdir("/mnt/hgfs/Program/2");
	sock = socket(AF_INET, SOCK_STREAM, 0);
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
	/*send(sock,argv[3],strlen(argv[3]),0);
	sleep(1);
	int n;
	int fd=open(argv[3],O_RDWR);*/
	pthread_t pt;
	while(1)
	{		
				
		char buf[512]={0};
		gets(buf);
		if(strncmp(buf,"get",3)==0)
		{
			char *p=buf;p+=4;

			send(sock,buf,strlen(buf),0);
			pthread_create(&pt,NULL,func2,(void*)p);
		}
		if(strncmp(buf,"put",3)==0)
		{
			char *p=buf;p+=4;
			send(sock,buf,strlen(buf),0);
			pthread_create(&pt,NULL,func3,(void*)p);
		}
		if(strcmp(buf,"ls")==0)	
		{
			send(sock,buf,2,0);
			pthread_create(&pt,NULL,func1,NULL);
		}
		pthread_join(pt,NULL);

	}
	close(sock);
}



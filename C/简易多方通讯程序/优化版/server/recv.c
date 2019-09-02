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
#include "linklist.h"
struct headnode* tou;



	int connect_fd[10];
	int i=0;
	
	pthread_t pt[10];
	int k=0;

void* func(void* data)
{
	int a=i;
	int flag=0;

	while(1)
	{
		while(1)
			{
				char buf[32] = {0};
				int re=recv(connect_fd[a],buf,32,0);
				if(re==0)
					{
						printf("%d号下线\n",a);
						shanchu(tou,a);
						//printf("haha\n");
						flag=1;
						break;
						continue;
					}
				if(strcmp(buf,"offline")==0)
				{
					printf("%d号下线\n",a);

					break;
				}
				if(strcmp(buf,"#poweroff")==0)
				{
					printf("%d号下线\n",a);
					shanchu(tou,a);
					//printf("haha\n");
					flag=1;
					break;
				}
				
				if(strncmp(buf,"##",2)!=0)
				{
					char buf1[100];
					sprintf(buf1,"群：%d号说:%s",a,buf);
					/*int j;
					for(j=0;j<10;j++)
					{
						if(j==a)continue;
						send(connect_fd[j],buf1,strlen(buf1),0);
					}*/
					struct node*s=tou->first;
					while(s!=NULL)
					{
						if(s->num==a)
						{
							s=s->next;
							continue;
						}

						send(s->fd,buf1,strlen(buf1),0);
						s=s->next;
					}
				}
				else
				{
					char*p=buf;p+=2;
					int b=(int)(*p)-48;
					//printf("%d\n", b);
					char buf1[100];
					p+=2;
					sprintf(buf1,"%d号对您说:%s",a,p);
					send(connect_fd[b],buf1,strlen(buf1),0);
				}
			}
			while(!flag)
			{
				
				char buf2[32]={0};
				recv(connect_fd[a],buf2,32,0);
				if(strcmp(buf2,"online")==0)
				{
					printf("%d号上线\n",a);
					break;
				}
				
			}
			if(flag)break;
		}
	

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


	tou=create_head();


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
	
	while (1)
	{
		struct sockaddr_in sa1;
		socklen_t addrlen = sizeof(sa1); 
		connect_fd[i] = accept(sock, (struct sockaddr *)&sa1, &addrlen);
		struct node* p=malloc(sizeof(*p));
		p->next=p->prev=NULL;p->fd=connect_fd[i];p->num=i;
		add_node(tou,p);

	
		//printf("receive from %s [port :%d]\n", inet_ntoa(sa1.sin_addr),ntohs(sa1.sin_port));
		printf("%d号上线\n",i );
		pthread_create(&pt[k],NULL,func,NULL);
		sleep(1);
		k++;
		i++;
		
			//子进程与客户端收发数据
			
		
		
		
	}
}

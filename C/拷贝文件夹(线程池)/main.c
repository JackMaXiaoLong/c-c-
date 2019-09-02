#include "thread_pool.h"
#include <dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#include<bits/pthreadtypes.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<signal.h>
//多线程实现文件夹的拷贝

#include <stdbool.h>
//创建一个线程池
struct thread_pool pool;

	//线程池的初始化


struct fd
{
	int f1;
	int f2;
};
int copyfile(int fd_src, int fd_dst)
{
	
	int nread, nwrite, num=0;

	while(1)
	{			
		char buf[512]={0};
		nread = read(fd_src, buf, 512);
		if(nread == 0)
			break;
		else if(nread == -1)
		{
			perror("read error");
			return -1;
		}
		nwrite = write(fd_dst, buf, nread);
		num += nwrite;		
	}	
	close(fd_src);
	close(fd_dst);
	return num;
}

void *func(void *data)
{
	int fd[2];
	fd[0] =((struct fd*)data)->f1;
	fd[1] =((struct fd*)data)->f2;
	copyfile(fd[0], fd[1]);
	
	
	free(data);
}

void cp_dir(char*src,char*dst)
{
	//打开这个目录
	

	DIR*dir = opendir(src);
	//DIR*dir1 = opendir(dst);
	//读取目录
	while(1)
	{
		struct dirent*dirent = readdir(dir);
		if(dirent == NULL)
		{
			break;
		}
		if(strcmp(dirent->d_name,".")==0||strcmp(dirent->d_name,"..")==0)
		{
			continue;
		}
		//判断文件类型
			//普通文件：拷贝
		struct stat st;
		char wondelfulname[512]={0};
		sprintf(wondelfulname, "%s/%s", src, dirent->d_name);
		stat(wondelfulname, &st);
		//int fd[2];
		if(S_ISREG(st.st_mode))
		{
			//printf("hahahah\n");
			struct fd*p=(struct fd*)malloc(sizeof(*p));
			p->f1=open(wondelfulname, O_RDWR);
			char dst1[512]={0};
			sprintf(dst1, "%s/%s", dst, dirent->d_name);
			p->f2=open(dst1, O_RDWR|O_CREAT,0777);
		//	printf("%s\n",wondelfulname);
			add_task(&pool,func,(void*)p);
			//sleep(1);
			//printf("dsadfas\n");
		}	
		else if(S_ISDIR(st.st_mode))
		{	//printf("qqqqq\n");
			char dst1[512]={0};
			sprintf(dst1, "%s/%s", dst, dirent->d_name);
			mkdir(dst1,0777);
			cp_dir(wondelfulname,dst1);
		}				
	}
	closedir(dir);
	//closedir(dir1);
}

main()
{
	init_pool(&pool,10);	

	char *a="/mnt/hgfs/Program/1";
	char *b="/mnt/hgfs/Program/2";
	char *p=a+strlen(a)-1;
	while(*p!='/')p--;
	char buf[512]={0};
	sprintf(buf,"%s%s",b,p);
	mkdir(buf,0777);
	cp_dir(a,buf);
	destroy_pool(&pool);
}

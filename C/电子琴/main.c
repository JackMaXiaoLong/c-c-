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
#include "thread_pool.h"
int *p = NULL;

struct thread_pool pool;

char *mp3[]=
{
	"madplay -Q d1.mp3 &",
	"madplay -Q d2.mp3 &",
	"madplay -Q d3.mp3 &",
	"madplay -Q d4.mp3 &",
	"madplay -Q d5.mp3 &",
	"madplay -Q d6.mp3 &",
	"madplay -Q d7.mp3 &",
	"madplay -Q d8.mp3 &",
	"madplay -Q d9.mp3 &",
	"madplay -Q d10.mp3 &",
	"madplay -Q d11.mp3 &",
	"madplay -Q d12.mp3 &"
};

void* func(void *data)
{
	int i=*((int*)data);	
	display_bmp2("on.bmp",(i-1)*65+10,195);
	system("killall -9 madplay");
	system(mp3[i-1]);
	usleep(50000);
	display_bmp2("off.bmp",(i-1)*65+10,195);
}

int main()
{
	init_pool(&pool,12);
	int fd=lcd_init();
	display_bmp("beijing.bmp");
	display_bmp("bar.bmp");
	display_bmp2("off.bmp",10,195);
	display_bmp2("off.bmp",75,195);
	display_bmp2("off.bmp",140,195);
	display_bmp2("off.bmp",205,195);
	display_bmp2("off.bmp",270,195);
	display_bmp2("off.bmp",335,195);
	display_bmp2("off.bmp",400,195);
	display_bmp2("off.bmp",465,195);
	display_bmp2("off.bmp",530,195);
	display_bmp2("off.bmp",595,195);
	display_bmp2("off.bmp",660,195);
	display_bmp2("off.bmp",725,195);
	while(1)
	{
		int a=area_panduan();
		if(a==1)
		{
			add_task(&pool,func,(void*)(&a));			
		}
		if(a==2)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==3)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==4)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==5)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==6)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==7)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==8)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==9)
		{
			add_task(&pool,func,(void*)(&a));
		}
		if(a==10)
		{
			add_task(&pool,func,(void*)(&a));	
		}
		if(a==11)
		{
			add_task(&pool,func,(void*)(&a));	
		}
		if(a==12)
		{
			add_task(&pool,func,(void*)(&a));	
		}
	}
	destroy_pool(&pool);
	close(fd);
}
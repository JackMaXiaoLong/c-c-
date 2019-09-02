#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h> /*perror, errno ,...*/
#include <stdlib.h>/*abs的头文件*/
#include <stdio.h> /*printf*/
#include <sys/mman.h>
#include <linux/input.h>
#include <stdlib.h>
#include <time.h> /*time */
#include <string.h>
#include <linux/fb.h>
#include <pthread.h>
#include <dirent.h> 
#include "linklist.h" 
#include "lcd.h"
#include "jpg_display.h"
  


int *p = NULL;//指向第一个显存的地址
int i=0;//数组元素个数
char *c[]={
	"madplay -Q 1.mp3 &",
	"madplay -Q 2.mp3 &",
	"madplay -Q 3.mp3 &"
};


int main()
{
	struct headnode* currenthead=create_head();
	find_picture("/test/picture",currenthead);
	int fd=lcd_init();
	struct node* p3=currenthead->first;
	int flag=0;	
	int k=0;	
	i=geshu(currenthead);
	currenthead->last->next=currenthead->first;
	currenthead->first->prev=currenthead->last;
	while(1)
	{
		display_bmp("start.bmp");
		display_bmp2("enter.bmp", 320, 340);
		while(1)
		{
			if(click_panduan(320, 340, 120, 80))
			{
				display_bmp2("loading1.bmp", 320, 340);
				usleep(600000);
				display_bmp2("loading2.bmp", 320, 340);
				usleep(600000);
				display_bmp2("loading3.bmp", 320, 340);
				usleep(600000);
				display_bmp2("loading4.bmp", 320, 340);
				usleep(600000);

				display_bmp2("loading2.bmp", 320, 340);
				usleep(600000);
				display_bmp2("loading3.bmp", 320, 340);
				usleep(600000);
				display_bmp2("loading4.bmp", 320, 340);
				usleep(750000);
				display_bmp2("yes.bmp", 320, 340);
				usleep(900000);
		 start: if(strcmp(p3->data+strlen(p3->data)-4,".bmp") == 0 )
				display_bmp(p3->data);
				if(strcmp(p3->data+strlen(p3->data)-4,".jpg") == 0 )
				display_jpg(p3->data);
				system(c[k]);
				break;
			}

		}

		while(1)
		{
			int a=direction_panduan();
			if(a==3)
			{
				p3=p3->next;
				if(strcmp(p3->data+strlen(p3->data)-4,".bmp") == 0 )
				display_bmp(p3->data);
				if(strcmp(p3->data+strlen(p3->data)-4,".jpg") == 0 )
				display_jpg(p3->data);
			}
			if(a==4)
			{
				p3=p3->prev;
				if(strcmp(p3->data+strlen(p3->data)-4,".bmp") == 0 )
				display_bmp(p3->data);
				if(strcmp(p3->data+strlen(p3->data)-4,".jpg") == 0 )
				display_jpg(p3->data);
			}
			if(a==1){system("killall -9 madplay");break;}
			if(a==2)
			{
				flag=1;
				break;
			
			}

		}
		if(flag)
		{
			flag=0;

			break;
		}

	}
	display_bmp("end.bmp");
	while(1)
	{
		int b=direction_panduan();
		if(b==2)break;
		if(b==1)goto start;
		if(b==3){
			system("killall -9 madplay");
			if(k==2)k=-1;
			system(c[++k]);}
		if(b==4)
			{system("killall -9 madplay");
			if(k==0)k=3;
			system(c[--k]);}
	}
	system("killall -9 madplay");
	display_bmp("poweroff.bmp");
	sleep(2);
	display_bmp("black.bmp");

	system("killall -9 maxiaoc");


	lcd_uinit(fd);
}
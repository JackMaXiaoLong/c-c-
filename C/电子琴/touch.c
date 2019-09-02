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
#include "touch.h"


int touch_panduan()
{
	struct input_event et;
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 error");
		return -1;
	}
	
	while(1)
	{
		int r = read(fd,&et,sizeof(et));

		if(r == sizeof(et))
		{

			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)
			{
			
					return 1;
				
			}
		}
	}
	
	close(fd);
	return 0;
}

int click_panduan(int x, int y, int width, int height)
{
	struct input_event et;
	int x0, y0;
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 error");
		return -1;
	}
	
	while(1)
	{
		int r = read(fd,&et,sizeof(et));

		if(r == sizeof(et))
		{
			if(et.type==EV_ABS && et.code==ABS_X )
			{
				x0 = et.value;
			}

			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				y0 = et.value;	
			}



			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)
			{
				if ((x < x0) && (x0 < (x + width)) && (y < y0) && (y0 < (y + height))) 
				{   
					return 1;
				}
			}
		}
	}
	
	close(fd);
	return 0;
}

int direction_panduan()
{
	struct input_event et;
	int x0,x1,y0,y1;
	int touch_flagx=1,touch_flagy=1;
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 error");
		return -1;
	}	
	while(1)
	{
		int r = read(fd,&et,sizeof(et));
		if(r == sizeof(et))
		{
			if(et.type==EV_ABS && et.code==ABS_X )
			{
				if(touch_flagx==1)
				{
					touch_flagx = 0;
					x0 = et.value;
				}
				x1 = et.value;
			}
			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				if(touch_flagy==1)
				{
					touch_flagy = 0;
					y0 = et.value;
				}
				y1 = et.value;
			}
			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)
			{
				touch_flagx = 1;
				touch_flagy = 1;
				if(abs(y1-y0) > (100+abs(x1-x0)))
				{
					if(y1 < y0)
					{
						close(fd);
						return 1;
					}
					else
					{					
						close(fd);
						return 2;
					}
				}
				else if(abs(x1-x0) > (50+abs(y1-y0)))
				{
					if(x1 < x0)
					{
						close(fd);
						return 3;
					}
					else 
					{
						close(fd);
						return 4;
					}
				}
			}
		}
	}
	
	close(fd);
	return 0;
}




int area_panduan()
{
	struct input_event et;
	int x0, y0;
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 error");
		return -1;
	}
	
	while(1)
	{
		int r = read(fd,&et,sizeof(et));

		if(r == sizeof(et))
		{
			if(et.type==EV_ABS && et.code==ABS_X )
			{
				x0 = et.value;
			}

			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				y0 = et.value;	
			}
	


			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)
			{
				if ((10 < x0) && (x0 <75) && (195 < y0) && (y0 < 475)) 
				{   
					return 1;
				}
				if ((75 < x0) && (x0 <140) && (195 < y0) && (y0 < 475)) 
				{   
					return 2;
				}
				if ((140 < x0) && (x0 <205) && (195 < y0) && (y0 < 475)) 
				{   
					return 3;
				}
				if ((205 < x0) && (x0 <270) && (195 < y0) && (y0 < 475)) 
				{   
					return 4;
				}
				if ((270 < x0) && (x0 <335) && (195 < y0) && (y0 < 475)) 
				{   
					return 5;
				}
				if ((335 < x0) && (x0 <400) && (195 < y0) && (y0 < 475)) 
				{   
					return 6;
				}
				if ((400 < x0) && (x0 <465) && (195 < y0) && (y0 < 475)) 
				{   
					return 7;
				}
				if ((465 < x0) && (x0 <530) && (195 < y0) && (y0 < 475)) 
				{   
					return 8;
				}
				if ((530 < x0) && (x0 <595) && (195 < y0) && (y0 < 475)) 
				{   
					return 9;
				}
				if ((595 < x0) && (x0 <660) && (195 < y0) && (y0 < 475)) 
				{   
					return 10;
				}
				if ((660 < x0) && (x0 <725) && (195 < y0) && (y0 < 475)) 
				{   
					return 11;
				}
				if ((725 < x0) && (x0 <790) && (195 < y0) && (y0 < 475)) 
				{   
					return 12;
				}
			}
		}
	}
	
	close(fd);
	return 0;
}
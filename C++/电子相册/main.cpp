#include<iostream>
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
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<linux/socket.h>
#include<errno.h>
#include<dirent.h>
#include<sys/mman.h>
#include<linux/input.h>
#include<time.h>
#include<linux/fb.h>
#include"color.h"
#include"dir.h"
#include"image.h"
#include"PaintDevice.h"
#include"Painter.h"
#include"StringList.h"
using namespace std;

int main()
{
	PaintDevice device("/dev/fb0");
	Painter pter(&device);
	Dir dir("/test/xiangce");
	StringList L = dir.EntryList();
	Image tupian1("/test/xiangce/poweroff.bmp");
	pter.draw_image(tupian1,0,0);
	// while(1)
	// {
	// 	for(Iterator it=L.begin();it!=L.end();++it)
	// 	{
	// 		Image tupian(*it);
	// 		//cout << *it << endl;
	// 		//Image& haha=tupian;
	// 		pter.draw_image(tupian,0,0);
	// 		sleep(1);
	// 	}
	// }
}
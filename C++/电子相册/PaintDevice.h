#ifndef _PAINTDEVICE_H
#define _PAINTDEVICE_H
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
using namespace std;
class PaintDevice
{
	public:
		PaintDevice(string name);
		~PaintDevice();
		int*get_addr();
		int width()const;
		int height()const;
		
	private:
		void open();
		void get_info();//获取设备的基本属性
		void mmap();
		
	private:
		int fd;
		int w;
		int h;
		int dep;//色深
		int size;
		string name;
		int *addr;
};

#endif 
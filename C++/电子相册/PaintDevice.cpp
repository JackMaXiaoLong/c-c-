#include "PaintDevice.h"


PaintDevice::PaintDevice(string name):name(name)
{
	open();
	get_info();
	mmap();
}

void PaintDevice::open()
{
	fd = ::open(name.c_str(),O_RDWR);
	if(fd == -1)
	{
		throw "open device error";
	}
}

void PaintDevice::get_info()
{
	
	struct fb_var_screeninfo info;
	int re = ioctl(fd,FBIOGET_VSCREENINFO,&info);
	if(re == -1)
	{
		throw "get screen info error";
	}
	
	w = info.xres;
	h = info.yres;
	dep = info.bits_per_pixel;//32
	size = w*h*dep/8;
}

void PaintDevice::mmap()
{
	addr = static_cast<int*>(::mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0));
	if(addr == MAP_FAILED)
	{
		throw "mmap error";
	}
}

PaintDevice::~PaintDevice()
{
	munmap(addr,size);
	close(fd);
}

int*PaintDevice::get_addr()
{
	return addr;
}
int PaintDevice::width()const
{
	return w;
}
int PaintDevice::height()const
{
	return h;
}



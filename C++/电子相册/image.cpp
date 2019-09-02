#include"image.h"

Image::Image(string name):name(name)
{
	//打开
	fd = open(name.c_str(),O_RDONLY);
	if(fd == -1)
	{
		throw "open image error";
	}
	
	
	//获取信息：宽，高，色深，像素数据
	lseek(fd,0x12,SEEK_SET);
	read(fd,&w,4);
	read(fd,&h,4);
	
	//读取色深
	lseek(fd,0x1c,SEEK_SET);
	read(fd,&dep,2);
	
	//大小
	size = w*h*dep/8;
	
	//读取像素数据
	lseek(fd,0x36,SEEK_SET);
	data = new char[size];
	read(fd,data,size);
	cout << "open ok" << w <<""<<h <<" "<<size <<" "<<endl;
}
Image::~Image()
{
	delete []data;
	close(fd);
}
int Image::width()const
{
	return w;
}
int Image::height() const
{
	return h;
}

char *Image::get_data()
{
	return data;
}

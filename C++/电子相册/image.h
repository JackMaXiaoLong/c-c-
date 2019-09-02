#ifndef _IMAGE_H
#define _IMAGE_H
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


using namespace std;

class Image
{
	public:
		Image(string name);//打开 获取属性，以及像素数据
		~Image();
		int width()const;
		int height()const;
		char*get_data();
	private:
		string name;
		int fd;
		int w;
		int h;
		int size;
		short dep;//色深
		char*data;
		
};



#endif
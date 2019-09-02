#include "Painter.h"
#include<stdio.h>

Painter::Painter(PaintDevice *dev):dev(dev)
{
	
}

Painter::~Painter()
{
	
}

void Painter::draw_point(int x,int y,Color color)
{
	*(dev->get_addr()+x+y*dev->width()) = color.toInt();
}
void Painter::draw_circle(int x,int y,int r,Color color)
{
	for(int i = 0;i < dev->height();i++)//纵坐标
	{
		for(int j = 0;j < dev->width();j++)//横坐标
		{
			if(((i-y)*(i-y)+(j-x)*(j-x))<= r*r)
			{
				draw_point(j,i,color);
			}
		}
	}
}

void Painter::draw_rect(int x,int y,int w,int h,Color color)
{
	for(int i = 0;i<h;i++)
	{
		for(int j = 0;j < w;j++)
		{
			draw_point(j+x,i+y,color);
		}
	}
}

void Painter::draw_image(Image& img,int x,int y)
{
	char r,g,b;
	char*p = img.get_data();
	
	for(int i = 0;i<img.height();i++)
	{
		for(int j = 0;j <img.width();j++)
		{
			b = *p++;
			g = *p++;
			r = *p++;
			Color color(r,g,b);
			//printf("fa %d\n",*p);
			draw_point(j+x,y-i+img.height()-1,color);
		}
	}
}


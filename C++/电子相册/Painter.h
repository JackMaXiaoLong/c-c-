#ifndef _PAINTER_H
#define _PAINTER_H
#include "color.h"
#include "PaintDevice.h"
#include "image.h"
class Painter
{
	public:
		Painter(PaintDevice *dev);
		~Painter();
		void draw_point(int x,int y,Color color);
		void draw_circle(int x,int y,int r,Color color);
		void draw_rect(int x,int y,int w,int h,Color color);
		void draw_image(Image& img,int x,int y);
		
		
	private:
		PaintDevice *dev;
	
};

#endif
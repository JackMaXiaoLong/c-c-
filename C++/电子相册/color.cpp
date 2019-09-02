#include "color.h"
Color::Color(int r,int g,int b,int a):a(a),r(r),g(g),b(b)
{
	
}

Color::~Color()
{
	
}

int Color::toInt()
{
	int color;
	color = a << 24 | r << 16 | g << 8 | b;
	return color;
}
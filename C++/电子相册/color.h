#ifndef _COLOR_H
#define _COLOR_H
class Color
{
	public:
		Color(int r,int g,int b,int a = 0);
		~Color();
		int toInt();
		
		
	private:
		int a;
		int r;
		int g;
		int b;
};



#endif
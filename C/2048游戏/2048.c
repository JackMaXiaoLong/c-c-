
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

#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define LCD_BYTESPERPIXEL   4  //每个像素点占几个字节

#define LCD_SIZE   (LCD_WIDTH * LCD_HEIGHT *LCD_BYTESPERPIXEL)

#define MATRIX_X0    0     //整个棋盘的左上角顶点的x轴坐标
#define MATRIX_Y0    0    //整个棋盘的左上角顶点的y轴坐标
#define PIECE_SIZE    100   //棋子的像素点大小

#define BOARDSIZE  4  // 整个棋盘的大小BORADSIZE * BOARDSIZE
#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4

char *model = "tradition";

unsigned char bmpdata[LCD_SIZE] ;  //读bmp 图片数据临时数组 
int game_over = 0;//游戏结束的标志，1表示游戏结束
					//0表示游戏不结束


//棋盘矩阵
int matrix[BOARDSIZE][BOARDSIZE] =
{
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0	
};

int *plcd = NULL;//指向framebuffer(屏幕显存第一个点的内存)

/*
	lcd_draw_point:在屏幕坐为(x, y)这个点，填充color 这个颜色值。
	@x: x轴坐标
	@y:y轴坐标
	@color:要填充的辨色值

	返回值:
		无返回值。
*/
void lcd_draw_point(int x, int y, int color)
{
	int *p = plcd;

	if (x >= 0 && x < LCD_WIDTH && y>=0 && y <LCD_HEIGHT)
	{
		*(p +LCD_WIDTH*y + x) = color;
	}
	else
	{
		printf("the index is not correct\n");
	}
}


/*
	lcd_draw_rect: 在屏幕上画一个矩形，并且用
		color这种颜色填充该矩形。
	@x0: 该矩形的左上角的那个点x轴坐标
	@y0:该矩形的左上角的那个点y轴坐标
	@w:该矩形的宽
	@h:该矩形的高
	@color:该矩形要填充的辨色值

	返回值:
		无返回值。		
*/
void lcd_draw_rect(int x0, int y0, int w, int h, int color)
{
	if (x0 < 0 || y0 < 0 || w < 0 || h <0)
		return;

	if ((x0 + w >LCD_WIDTH) || (y0+h) > LCD_HEIGHT)
	{
		return;
	}

	int x, y;

	for (y = y0; y < y0 + h; y++)
	{
		for (x = x0; x < x0 + w; x++)
		{
			lcd_draw_point(x, y  ,  color);
		}
	}

}



//将所有的图片名保存到一个数组中
const char * bmpfiles[] =	
{
	"military_font_7_2.bmp", // 2
	"military_font_7_4.bmp", //   4
	"military_font_7_8.bmp", //8
	"military_font_7_16.bmp",  // 16
	"military_font_7_32.bmp",
	"military_font_7_64.bmp",
	"military_font_7_128.bmp",
	"military_font_7_256.bmp",
	"military_font_7_512.bmp",
	"military_font_7_1024.bmp",
	"military_font_7_2048.bmp",
	"military_font_7_4096.bmp",
	"military_font_7_8192.bmp",
	"military_font_7_16384.bmp",
	"military_font_7_32768.bmp",
	"military_font_7_65536.bmp",
	"gameover.bmp",
	"restart.bmp",
	"s.bmp",
	"a.bmp",
	"b.bmp",
	"changemodel.bmp"
};



/*
	get_bmpfiles_index:根据你要显示的数字(2,4,8,,16,...)
		返回你对应的文件名的下标
	返回值:
		返回 x对应的文件名在数组bmpfiles的下标
*/
int get_bmpfiles_index(int x)
{
	int i=0,t=1;
	while(t!=x)
	{
		t =t *2;
		i++;
	}

	return i-1;

}


/*draw_bmp_byname:在(x0,y0)处显示 1.bmp 图片，(x0,y0)为图片最左上角的点的坐标*/
void draw_bmp_byname(const char *bmpfile, int x0, int y0)
{
	int fd = open(bmpfile,O_RDONLY);
	if(fd == -1)
	{
		perror("open bmp error");
		return ;
	}
	
	char data[4];
	
	/*读取魔数，判断是不是bmp格式*/
	read(fd,data,2);
	if(data[0] != 0x42 || data[1] != 0x4d )
	{
		printf("pitcture formate error!\n");
		return ;
	}
	
	/*读取像素数组的偏移量*/
	lseek(fd,0x0a,SEEK_SET);
	read(fd,data,4);
	int offset = (data[3] & 0xff)<<24 | (data[2] & 0xff)<<16 | (data[1] & 0xff)<< 8 |  data[0]; //小端模式
	printf("offset is %d\n",offset);
	
	/*读取图片的宽度*/
	lseek(fd,0x12,SEEK_SET);
	read(fd,data,4);
	int x = (data[3] & 0xff)<<24 | (data[2] & 0xff)<<16 | (data[1] & 0xff)<< 8 |  data[0]; //小端模式
	printf("x is %d\n",x);
	
	/*读取图片的高度*/
	lseek(fd,0x16,SEEK_SET);
	read(fd,data,4);
	int y = (data[3] & 0xff)<<24 | (data[2] & 0xff)<<16 | (data[1] & 0xff)<< 8 |  data[0]; //小端模式
	printf("y is %d\n",y);
	
	/*读取图片的色深*/
	lseek(fd,0x1c,SEEK_SET);
	read(fd,data,2);
	int color_depth = (data[1] & 0xff)<< 8 |  data[0]; //小端模式
	printf("color_depth is %d\n",color_depth);
	
	/*读取像素数组的数据*/
	char tmp[x*abs(y)*color_depth/8];//abs用来求整数的绝对值，
	char *p = tmp;
	lseek(fd,offset,SEEK_SET);
	read(fd,tmp,x*abs(y)*color_depth/8);
	
	/*利用画点函数把图片显示出来*/
	int w,h;
	for(h=0;h<abs(y);h++)
	{
		for(w=0;w<x;w++)
		{
			char a,r,g,b;
			int color;
			b = *(p++);
			g = *(p++);
			r = *(p++);
			a = (color_depth == 24) ? 0 : *(p++);
			
			color = (a & 0xff)<<24 | (r & 0xff)<<16 | (g & 0xff)<<8 |(b & 0xff);
			
			lcd_draw_point(x0 + w,y0 + ((y<0) ? h : y-1-h),color);			
		}
		p += (4-(x*color_depth/8%4))%4;//每一行以4字节对齐方式保存，多的空间要跳过
	}	
	close(fd);
}

/*
	draw_matrix:把棋盘矩阵在屏幕上显示出来
*/
void draw_matrix()
{
	int i, j;
	for (i = 0; i < BOARDSIZE; i++)
	{
		for (j = 0; j < BOARDSIZE;j++)
		{
			int x0, y0;
			x0 = 185;//棋盘矩阵左上角那个点的x轴坐标
			y0 = 25;//棋盘矩阵左上角那个点的y轴坐标
			if (matrix[i][j] == 0)
			{
				lcd_draw_rect(x0+j*110,  y0+i*110,  PIECE_SIZE, PIECE_SIZE, 0xb4eeb4);//如果此处元素的值为0，那么
								//就显示
			}
			else
			{
				int f_index = get_bmpfiles_index(matrix[i][j]);
				// 拼接路径   tradition/xx.bmp
				char *path = (char *) malloc(strlen(model) + strlen("/") + strlen(bmpfiles[f_index]));
				sprintf(path, "%s/%s", model, bmpfiles[f_index]);
				draw_bmp_byname(path,	x0+j*110,  y0+i*110);
			}
		}
	}
}


/*
	get_zero_num:求棋盘矩阵里面有多少个0
	返回值:
		返回棋盘矩阵中0的个数
*/
int get_zero_num()
{
	int z = 0;//棋盘矩阵中元素为0的个数
	int i, j;

	for (i = 0; i < BOARDSIZE; i++)
	{
		for (j = 0; j < BOARDSIZE; j++)
		{
			if (matrix[i][j] == 0)
			{
				z++;
			}
		}
	}

	return z;
}

/*
	set_matrix:给棋盘矩阵第z个0的位置，填充一个
		值s
*/
void set_matrix(int z,  int s)
{
	int i, j;
	int k = 0 ;//0的个数

	for (i = 0; i < BOARDSIZE ;i++)
	{
		for (j = 0; j < BOARDSIZE; j++)
		{
			if (matrix[i][j] == 0)
			{
				k++;
				if (k == z)
				{
					matrix[i][j] = s;
					return ;
				}
			}
			
		}
	}

}


/*
	init_matrix:初始化棋盘矩阵
			在任意x个位置，填充x个数字(2,4,8)
*/

void init_matrix()
{
	//规则x >= 1,x <= 3
	int x = (random() % 3) + 1;

	int i;

	/*
	step1:随机产生x个数字，并填充到棋盘矩阵中去
	*/

	for(i = 0; i < x; i++)
	{
		int pos = (random() % get_zero_num()) + 1;

		int s[] = {2, 4, 8, 2};
		int s_i = (random() % 4);

		set_matrix(pos, s[s_i]);
	}

	/*
	step 2: 绘制棋盘矩阵
	*/
	draw_matrix();

}


/*
	get_finger_direction:获取手指在触摸屏上面的滑动方向
	返回值:
		MOVE_LEFT:手指向左移动
		MOVE_RIGHT:手指向右移动
		MOVE_UP:手指向上移动
		MOVE_DOWN:手指向下移动
*/
int get_finger_direction()
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
			printf("type:%ld code:%ld  value:%d\n",et.type,et.code,et.value);
			if(et.type==EV_ABS && et.code==ABS_X )
			{
				if(touch_flagx==1)
				{
					touch_flagx = 0;
					x0 = et.value;//获取横坐标
				}
				x1 = et.value;
			}
			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				if(touch_flagy==1)
				{
					touch_flagy = 0;
					y0 = et.value;//获取纵坐标
				}
				y1 = et.value;
			}
			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)//松开
			{
				touch_flagx = 1;
				touch_flagy = 1;
				printf("=====(%ld,%ld)==>(%ld,%ld)=====\n",x0,y0,x1,y1);
				if(abs(y1-y0) > 2*abs(x1-x0))
				{
					if(y1 < y0)
					{
						printf("========UP!========\n");
						close(fd);
						return MOVE_UP;
					}
					else
					{
						printf("========DOWN!======\n");
						close(fd);
						return MOVE_DOWN;
					}
				}
				else if(abs(x1-x0) > 2*abs(y1-y0))
				{
					if(x1 < x0)
					{
						printf("========LEFT!======\n");
						close(fd);
						return MOVE_LEFT;
					}
					else 
					{
						printf("========RIGHT!=====\n");
						close(fd);
						return MOVE_RIGHT;
					}
				}
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
				x0 = et.value;//获取横坐标
			}

			if(et.type==EV_ABS && et.code==ABS_Y)
			{
				y0 = et.value;//获取纵坐标	
			}



			if(et.type==EV_KEY && et.code==BTN_TOUCH && et.value==0)//松开
			{
				if ((x < x0) && (x0 < (x + width)) && (y < y0) && (y0 < (y + height))) 
				{   // 在区域里面
					return 1;
				}
			}
		}
	}
	
	close(fd);
	return 0;
}


/*
	fin_left:手指左划后棋子移动及合并的方式
*/
void fin_left()
{
	int i,j,k,t,n;//i决定行，j决定列
	for(i=0;i<BOARDSIZE;i++)
	{
		k=0;//第一次靠拢的下标
		t=0;//第二次靠拢的下标
		n=0;//合并的个数`
		for(j=0;j<BOARDSIZE;j++)
		{
			if(matrix[i][j] != 0)
			{
				matrix[i][k] = matrix[i][j];//并拢	
				if(matrix[i][k-1] == matrix[i][k])//合并
				{
					matrix[i][k-1] *= 2;
					matrix[i][k] = 0;
					n++;
				}
				k++;
			}
		}
		for(j=0;j<BOARDSIZE;j++)
		{
			if(matrix[i][j] != 0)//并拢 
			{
				matrix[i][t++] = matrix[i][j];	
			}		
		}
		for(j=k-n;j<BOARDSIZE;j++)//并拢后，后面的元素要清零
		{	
			matrix[i][j] = 0;
		}	
	}
}	

/*
	fin_right:手指上划后棋子移动及合并的方式
*/
void fin_right()
{	
	int i,j,k,t,n;//i决定行，j决定列
	for(i=0;i<BOARDSIZE;i++)
	{
		k=3;//第一次靠拢的下标
		t=3;//第二次靠拢的下标
		n=0;//合并的个数`
		for(j=3;j>=0;j--)
		{
			if(matrix[i][j] != 0)
			{
				matrix[i][k] = matrix[i][j];//并拢	
				if(matrix[i][k+1] == matrix[i][k])//合并
				{
					matrix[i][k+1] *= 2;
					matrix[i][k] = 0;
					n++;
				}
				k--;
			}
		}
		for(j=3;j>=0;j--)
		{
			if(matrix[i][j] != 0)//并拢 
			{
				matrix[i][t--] = matrix[i][j];	
			}		
		}
		for(j=k+n;j>=0;j--)//并拢后，后面的元素要清零
		{	
			matrix[i][j] = 0;
		}	
	}
} 
		
/*
	fin_up:手指上划后棋子移动及合并的方式
*/
void fin_up()
{
	int i, j, k, t, n; // i决定列，j决定行
    for (i = 0; i < BOARDSIZE; i++) {
        k = 0; // 第一次靠拢的下标
        t = 0; // 第二次靠拢的下标
        n = 0; // 合并的个数
        for (j = 0; j < BOARDSIZE; j++) {
            if (matrix[j][i] != 0) {
                matrix[k][i] = matrix[j][i]; // 并拢
                // 判断跟更上边相邻的一个方块的值是否一样，如果一样进行合并
                if (matrix[k - 1][i] == matrix[k][i]) {
                    matrix[k - 1][i] *= 2;   // 加倍
                    matrix[k][i] = 0;
                    n++;
                }
                k++;
            }
        }

        for (j = 0; j < BOARDSIZE; j++) {
            if (matrix[j][i] != 0) // 并拢
            {
                matrix[t++][i] = matrix[j][i];
            }
        }
        for (j = k - n; j < BOARDSIZE; j++) // 并拢后，后面的元素要清零
        {
            matrix[j][i] = 0;
        }
    }
}
	
/*
	fin_down:手指上划后棋子移动及合并的方式
*/
void fin_down()
{
	int i, j, k, t, n; // i决定行，j决定列
    for (i = 0; i < BOARDSIZE; i++) {
        k = BOARDSIZE - 1; // 第一次靠拢的下标
        t = BOARDSIZE - 1; // 第二次靠拢的下标
        n = 0; // 合并的个数
        for (j = BOARDSIZE - 1; j >= 0; j--) {
            if (matrix[j][i] != 0) {
                matrix[k][i] = matrix[j][i]; // 并拢
                // 判断跟更右边相邻的一个方块的值是否一样，如果一样进行合并
                if (matrix[k + 1][i] == matrix[k][i]) {
                    matrix[k + 1][i] *= 2;   // 加倍
                    matrix[k][i] = 0;
                    n++;
                }
                k--;
            }
        }

        for (j = BOARDSIZE - 1; j >= 0; j--) {
            if (matrix[j][i] != 0) // 并拢
            {
                matrix[t--][i] = matrix[j][i];
            }
        }
        for (j = k + n; j >= 0; j--) // 并拢后，后面的元素要清零
        {
            matrix[j][i] = 0;
        }
    }
}
/*
	change_matrix:根据手指滑动(direction)，
		变换棋盘矩阵
*/
int change_matrix()
{
	
	int direction = get_finger_direction();
	if (direction == MOVE_LEFT)
	{
		fin_left();
	}
	else if (direction == MOVE_RIGHT)
	{
		fin_right();
	}
	else if (direction == MOVE_UP)
	{
		fin_up();
	}
	else
	{
		fin_down();
	}
}

/*
	move_judge:判断是否还能移动
	return value:
		1 game over
		0 continue
*/
int move_judge()
{
	int i, j;
	if(get_zero_num() != 0)
	{
		return 0;
	}

	for(i = 0; i < BOARDSIZE; i++)
	{
		for(j = 0; j < BOARDSIZE ; j++)
		{
			if (j != BOARDSIZE -1)
			{
				if (matrix[i][j] == matrix[i][j+1])
				{
					return 0;
				}
			}

			if (i != BOARDSIZE - 1)
			{
				if (matrix[i][j] == matrix[i+1][j])
				{
					return 0;
				}
			}
		}
	}
	return 1;

}

/*
	rand1_matrix:移动之后随机产生一个数字填充到
	任意一个0的位置上
*/
void rand_matrix()
{
	int pos = (random() % get_zero_num()) + 1;
	int s[] = {2, 4, 8, 2};
	int s_i = (random() % 4);

	set_matrix(pos, s[s_i]);
}

void *restart(void *);
void *changemodel(void *);
int grade=0;

/************主函数**************/
int main()
{
	/*step 1:打开屏幕设备文件*/

	int fb = open("/dev/fb0",O_RDWR);
	if(fb == -1)
	{
		perror("open fb error");
		return -1;
	}
	/*step 2:获取屏幕信息*/
	struct fb_var_screeninfo fbinfo;
	ioctl(fb, FBIOGET_VSCREENINFO, &fbinfo);
	printf("%d * %d * %d/8\n",fbinfo.xres,fbinfo.yres,fbinfo.bits_per_pixel);
	
	/*step 3: 把屏幕设备文件映射到内存空间*/
	plcd = mmap(NULL,fbinfo.xres*fbinfo.yres*fbinfo.bits_per_pixel/8 , PROT_READ | PROT_WRITE, MAP_SHARED,fb,0);
	if(plcd == MAP_FAILED)
	{
		perror("mmap error");
		return -1;
	}
	
	
	lcd_draw_rect(0, 0, 800, 480, 0xffffff);//画背景	
	draw_bmp_byname(bmpfiles[17],20,20);
	draw_bmp_byname(bmpfiles[21],20,120);     // 换模式按钮
	srandom( time(NULL) ); //设置随机数种子，种子一样，产生的
						//随机数是一样的
	init_matrix();//初始化棋盘，并在任意x个位置，填充x个数字
	pthread_t restart_thread_id[2];
	pthread_create(&restart_thread_id[0], NULL, restart, NULL);
	pthread_create(&restart_thread_id[1], NULL, changemodel, NULL);
	int flag1;
	while(1)
	{
		
		
		
		game_over = move_judge();

		while (game_over == 0) //游戏没结束
		{
			/*
				step1: 把原来的矩阵的值保存在matrix_vi数组中
			*/
			int matrix_v1[BOARDSIZE][BOARDSIZE];
			int i, j;
			
			for (i = 0; i < BOARDSIZE;++i)
			{
				for (j = 0; j < BOARDSIZE; ++j)
				{
					matrix_v1[i][j] = matrix[i][j];
				}
			}
			
			/*
				step 2: 变换矩阵
			*/
			change_matrix();

			/*
				step 3:检查矩阵是否有变动
			*/
			int flag = 0;//矩阵是否有变化标志位
			for (i = 0; i < BOARDSIZE; ++i)
			{
				for (j = 0; j < BOARDSIZE; ++j)
				{
					if (matrix_v1[i][j] != matrix[i][j])
					{
						flag = 1;
						i = j = BOARDSIZE;
					}
				}
			}

			
			if (flag)//矩阵有变动
			{
				rand_matrix();
				grade++;
			} 
			draw_matrix();
			
			/*
				step 4:判断游戏是否结束
			*/
			game_over = move_judge();
			if(game_over)flag1=1;
		}
		
	if(flag1){	
	
		lcd_draw_rect(0, 0, 800, 480, 0xffffff);//画背景	
		draw_bmp_byname(bmpfiles[16],150,40);
		draw_bmp_byname(bmpfiles[17],20,20);
		draw_bmp_byname(bmpfiles[21],20,120);
		if(grade>299)
		{
			draw_bmp_byname(bmpfiles[18],680,200);
		}
		if(grade>99&&grade<=299)
		{
			draw_bmp_byname(bmpfiles[19],680,200);
		}
		if(grade>0&&grade<=99)
		{
			draw_bmp_byname(bmpfiles[20],680,200);
		}
		printf("Your grade is %d\n",grade);
		printf("Game Over~~\n");
		flag1=0;
	}	
}
		
	munmap(plcd, 800*480*4);
	close(fb);
	return 0;





}





void *restart(void *data)
{
	
	while(1)
	{
		int flag=0;
		flag=click_panduan(20,20,80,80);
		if (flag) 
		{
			int i,j;
			for(i=0;i<4;i++)
			{
				for(j=0;j<4;j++)
				{
					matrix[i][j] =0;
				}
			}
		
			grade=0;
			lcd_draw_rect(0, 0, 800, 480, 0xffffff);//画背景	
			draw_bmp_byname(bmpfiles[17],20,20);
			draw_bmp_byname(bmpfiles[21],20,120);     // 换模式按钮
			srandom( time(NULL) ); //设置随机数种子，种子一样，产生的
								//随机数是一样的
			init_matrix();//初始化棋盘，并在任意x个位置，填充x个数字
		}
	}
}


void *changemodel(void *data) {
	while(1) {
		int flag=0;
		flag=click_panduan(20,120,80,80);
		if (flag) 
		{
			// 切换模式
			if (model[0] == 't') {
				model = "special";
			} else {
				model = "tradition";
			}
			int i,j;
			for(i=0;i<4;i++)
			{
				for(j=0;j<4;j++)
				{
					matrix[i][j] =0;
				}
			}
		
			grade=0;
			lcd_draw_rect(0, 0, 800, 480, 0xffffff);//画背景	
			draw_bmp_byname(bmpfiles[17],20,20);
			draw_bmp_byname(bmpfiles[21],20,120);     // 换模式按钮
			srandom( time(NULL) ); //设置随机数种子，种子一样，产生的
								//随机数是一样的
			init_matrix();//初始化棋盘，并在任意x个位置，填充x个数字
		}
	}
}

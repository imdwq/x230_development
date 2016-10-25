//frambuffer.h
#ifndef _FRAMBUFFER_
#define _FRAMBUFFER

#include <linux/fb.h>

typedef struct fb_var_screeninfo vinfo;
typedef struct fb_bitfield color;
typedef struct
{
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	unsigned int alpha;
}color_8;

typedef int DrawFunc;
typedef DrawFunc (*IntFunc)(int);

void make_color(color_8 *out,unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);

void draw_point(unsigned char* buff, int x, int y, color_8 color);
void draw_func(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color, int x0, int y0);
void draw_func_0(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color);
void draw_line(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color);
void draw_block(unsigned char* buff, int xmin,int xmax, int ymin, int ymax, color_8 color);

unsigned char *fb_init();
void release_fb(unsigned char* buff);

void wipe_screen(unsigned char* buff);

int y_axis_trans(int y);
void zero_func_trans(int *x, int *y, int x0, int y0);

DrawFunc func_line(int x);
DrawFunc func_parabola(int x);
#endif

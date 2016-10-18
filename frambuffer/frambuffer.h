//frambuffer.h
#ifndef _FRAMBUFFER_
#define _FRAMBUFFER

#include <linux/fb.h>

typedef struct fb_var_screeninfo vinfo;
typedef struct fb_bitfield color;
typedef struct
{
	unsigned int red_8;
	unsigned int green_8;
	unsigned int blue_8;
}color_8;


void draw_point(unsigned char* buff, int x,int y , unsigned int red, unsigned int green, unsigned int blue);

void draw_block(unsigned char* buff, int xmin,int xmax, int ymin, int ymax, unsigned int red, unsigned int green, unsigned int blue);

unsigned char *fb_init();

void wipe_screen(unsigned char* buff);

void release_fb(unsigned char* buff);

#endif

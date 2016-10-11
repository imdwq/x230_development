//frambuffer.h
#ifndef _FRAMBUFFER_
#define _FRAMBUFFER_
#include<linux/fb.h>

typedef struct fb_var_screeninfo vinfo;
typedef struct fb_bitfield color;

void draw_point(unsigned char* buff, int x,int y , unsigned int red, unsigned int green, unsigned int blue);

void draw_block(unsigned char* buff, int xmin,int xmax, int ymin, int ymax, unsigned int red, unsigned int green, unsigned int blue);

void init(vinfo *fbinfo);

#endif

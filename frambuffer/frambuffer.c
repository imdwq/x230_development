#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "frambuffer.h"

#define BIT_TO_BYTE 8

static color red, green, blue, alpha;

static unsigned int xres;
static unsigned int yres;
static unsigned int bits_per_pixel;
static unsigned int screensize;
static unsigned int screenpixel;

unsigned char* fb_init()
{
	vinfo fbinfo;
	int fd = open ("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &fbinfo);

	xres = fbinfo.xres;
	yres = fbinfo.yres;
	bits_per_pixel = fbinfo.bits_per_pixel;
	screenpixel = xres * yres;

	red.offset = fbinfo.red.offset;
	red.length = fbinfo.red.length;
	blue.offset = fbinfo.blue.offset;
	blue.length = fbinfo.blue.length;
	green.offset = fbinfo.green.offset;
	green.length = fbinfo.green.length;
	alpha.offset = fbinfo.transp.offset;
	alpha.length = fbinfo.transp.length;
	
	screensize = screenpixel * (red.length + blue.length + green.length + alpha.length) / BIT_TO_BYTE;
	return ((unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
}

void draw_point(unsigned char *buff, int x, int y,unsigned int red_p, unsigned int green_p, unsigned int blue_p)
{
	unsigned int offset = (y * xres + x) * bits_per_pixel / BIT_TO_BYTE;
	*(unsigned char *)(buff + offset + red.offset / BIT_TO_BYTE) = red_p;
	*(unsigned char *)(buff + offset + green.offset / BIT_TO_BYTE) = green_p;
	*(unsigned char *)(buff + offset + blue.offset / BIT_TO_BYTE) = blue_p;

}

void draw_block(unsigned char *buff, int xmin,int xmax, int ymin, int ymax, unsigned int red_i, unsigned int green_i, unsigned int blue_i)
{
	int tmp;
	if (xmin > xmax)
	{
		tmp = xmin;
		xmin = xmax;
		xmax = tmp;
	}
	if (ymin > ymax)
	{
		tmp = ymin;
		ymin = ymax;
		ymax = tmp;
	}

	for (int j = ymax; j > ymin; j--)
	{
		for (int i = xmin; i < xmax; i++)
			draw_point(buff, i, j, red_i, green_i, blue_i);
	}
}

void wipe_screen(unsigned char* buff)
{
	memset(buff, 0, screensize);	
}

void release_fb(unsigned char* buff)
{
	munmap(buff, screensize); 
}


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

void make_color(color_8 *out, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	out->red = red;
	out->green = green;
	out->blue = blue;
	out->alpha = alpha;
}

void draw_point(unsigned char *buff, int x, int y, color_8 color)
{
	unsigned int offset = (y * xres + x) * bits_per_pixel / BIT_TO_BYTE;
	*(unsigned char *)(buff + offset + red.offset / BIT_TO_BYTE) = color.red;
	*(unsigned char *)(buff + offset + green.offset / BIT_TO_BYTE) = color.green;
	*(unsigned char *)(buff + offset + blue.offset / BIT_TO_BYTE) = color.blue;

}

void draw_func(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color, int x0, int y0)
{
	int x, y, tmp;
	if (xmin > xmax)
	{
		tmp = xmin;
		xmin = xmax;
		xmax = tmp;
	}
	for(tmp = xmin; tmp < xmax; tmp++)
	{
		y = y_axis_trans((*func)(tmp) + y0);
		x = tmp + x0;
		draw_point(buff, x, y, color);
	}
}

void draw_func_0(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color)
{
	draw_func(buff, func, xmin, xmax, color, 0 , 0);
}

void draw_line(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color)
{
	int tmp, x, y;
	double ratio;

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

	ratio = (double)(ymax - ymin) / (double)(xmax - xmin);

	for(x = xmin; x < xmax; x++)
	{
		y = (unsigned int)(ratio * (x - xmin) + ymin);
		draw_point(buff, x, y, color);
	}
}

void draw_block(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color) 
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
			draw_point(buff, i, j, color);
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

void zero_func_trans(int *x, int *y, int x0, int y0)
{
	*y = *y - y0;
	*x = *x - x0;
}

int y_axis_trans(int y)
{
	return yres - 1 - y;
}

DrawFunc func_line(int x)
{
	return x;
}

DrawFunc func_parabola(int x)
{
	return x*x;
}

DrawFunc func_parabola_0(int x)
{
	return x*x;
}

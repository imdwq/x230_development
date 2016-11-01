#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "frambuffer.h"

#define BIT_TO_BYTE 8

static color red, green, blue, alpha;

static uint32_t xres;
static uint32_t yres;
static uint32_t bits_per_pixel;
static uint32_t screensize;
static uint32_t screenpixel;

enum DrawFuction{line, sin, cos};


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

void make_color(color_8 *out, uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha)
{
	out->red = red;
	out->green = green;
	out->blue = blue;
	out->alpha = alpha;
}

void draw_point(unsigned char *buff, uint32_t x, uint32_t y, color_8 color)
{
	uint32_t offset = (y * xres + x) * bits_per_pixel / BIT_TO_BYTE;
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
		y = (uint32_t)(ratio * (x - xmin) + ymin);
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


int draw_BMP(unsigned char* buff, char path[])
{
	FILE *fp;

	if (fp = fopen(path, "r"))
	{
		printf("Open BMP!\n");

		BMP_head *head = (BMP_head *)malloc(sizeof(BMP_head));
		BMP_info *info = (BMP_info *)malloc(sizeof(BMP_info));

		if (1 != fread(head, sizeof(BMP_head), 1, fp))
		{

			return -1;
		}

		if (1 != fread(info, sizeof(BMP_info), 1, fp))
		{
			return -1;
		}

		if(fseek(fp, head->f_Offset, SEEK_SET))
		{
			return -1;
		}

		printf("BitCount = %d\n", info->BitCount);
		printf("Height = %d, Width = %d\n", info->Height, info->Width);	
		switch(info->BitCount)
		{
			case 24:draw_BMP_24(buff, fp, info);	break;
			case 32:draw_BMP_32(buff, fp, info);	break;
			default:break;

		}

		free(head);
		free(info);
		fclose(fp);
	}

	return 0;
}


void draw_BMP_24(unsigned char *buff, FILE *fp, BMP_info *info)
{
	printf("24bit!\n");
	int i;
	int x = 0;
	int y = 0;
	int width_offset;
	color_8 color;
	width_offset = info->Width * 3 / 4 * 4 + 4 - info->Width * 3;
	printf("offset = %d\n", width_offset);
	if (info->Height > 0)
	{
		for (y = info->Height; y > 0; y--)
		{
			for(x = 0; x < info->Width; x++)	
			{
				fread((void *)&(color.blue), 1, 1, fp);
				fread((void *)&(color.green), 1, 1, fp);
				fread((void *)&(color.red), 1, 1, fp);
				color.alpha = 0;
				draw_point(buff, x, y, color);
			}
			fseek(fp, width_offset, SEEK_CUR);
		}
	}
}	

void draw_BMP_32(unsigned char *buff, FILE *fp, BMP_info *info)
{
	printf("32bit!\n");
	int i;
	int x = 0;
	int y = 0;

	color_8 color;
	
	printf("Height = %d, Width = %d\n", info->Height, info->Width);	
	if (info->Height > 0)
	{
		for (y = info->Height; y > 0; y--)
		{
			for(x = 0; x < info->Width; x++)	
			{
				fread((void *)&(color.blue), 1, 1, fp);
				fread((void *)&(color.green), 1, 1, fp);
				fread((void *)&(color.red), 1, 1, fp);
				draw_point(buff, x, y, color);
			}
		}
	}
}

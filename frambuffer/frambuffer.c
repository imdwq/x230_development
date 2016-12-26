#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "frambuffer.h"

#define BIT_TO_BYTE 8

static fbcolor red, green, blue, alpha;

static uint32_t xres;
static uint32_t yres;
static uint32_t xvres;
static uint32_t yvres;
static uint32_t bits_per_pixel;
static uint32_t screensize;
static uint32_t screen0size;
static uint32_t screenpixel;
static int fd;
static int fd1;
static int fd2;

static enum ScreenNum SCREEN_USING;

unsigned char *fb_init()
{
	vinfo fbinfo;
	fd = open ("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &fbinfo);
	xres = fbinfo.xres;
	yres = fbinfo.yres;
	xvres = fbinfo.xres_virtual;
	yvres = yres * (V_SCREEN1 - V_SCREEN0 + 1);

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

	fbinfo.yres_virtual = yvres;

	screen0size = screenpixel * bits_per_pixel / BIT_TO_BYTE;
	screensize = screen0size * yvres / yres;
//	printf("screensize = %u, screen0size = %u\n", screensize, screen0size);

	fbinfo.activate = FB_ACTIVATE_FORCE;
	fbinfo.yoffset = 0;
	ioctl(fd, FBIOPUT_VSCREENINFO, &fbinfo);
	ioctl(fd, FBIOBLANK, FB_BLANK_UNBLANK);

//	ioctl(fd, FBIOPAN_DISPLAY, &fbinfo);
	ioctl(fd, FBIOPUT_VSCREENINFO, &fbinfo);
	SCREEN_USING = V_SCREEN0;
//	printf("vxres=%u, vyres= %u, yoffset=%u\n", fbinfo.xres_virtual, fbinfo.yres_virtual, fbinfo.yoffset);

	return ((unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
}

unsigned char *fb1_init()
{
	fd1 = open ("/dev/fb1", O_RDWR);
	vinfo fbinfo;
	ioctl(fd1, FBIOGET_VSCREENINFO, &fbinfo);
//	printf("yoffset=%u\n", fbinfo.xres, fbinfo.yres, fbinfo.yres_virtual, fbinfo.yoffset);
	fbinfo.activate = FB_ACTIVATE_FORCE;
	ioctl(fd1, FBIOPUT_VSCREENINFO, &fbinfo);
//	ioctl(fd1, FBIOPAN_DISPLAY, &fbinfo);
	ioctl(fd1, FBIOBLANK, FB_BLANK_UNBLANK);

	return ((unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0));
}

unsigned char *fb2_init()
{
	fd2 = open ("/dev/fb2", O_RDWR);
	vinfo fbinfo;
	ioctl(fd2, FBIOGET_VSCREENINFO, &fbinfo);
	fbinfo.activate = FB_ACTIVATE_FORCE;
	ioctl(fd2, FBIOPUT_VSCREENINFO, &fbinfo);
	ioctl(fd2, FBIOBLANK, FB_BLANK_UNBLANK);

	return ((unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0));
}


void wipe_screen(unsigned char* buff)
{
	unsigned char *tmp = buff + screen0size * (SCREEN_USING - V_SCREEN0);
	memset(tmp, 0, screen0size);
}

void wipe_screen0(unsigned char* buff)
{
	unsigned char *tmp = buff;
//	printf("%p\n", tmp);
	memset(tmp, 0, screen0size);
}

void wipe_oldscreen(unsigned char* buff)
{
	unsigned char *tmp = buff + screen0size * (V_SCREEN1 - SCREEN_USING);
	memset(tmp, 0, screen0size);
}

void wipe_allscreen(unsigned char* buff)
{
	memset(buff, 0, screensize);
}

void release_fb(unsigned char* buff)
{
	munmap(buff, screensize);
}

//DECEPATED:release_fb1
void release_fb1(unsigned char *buff)
{
	munmap(buff, screensize);
}

void switch_vscreen()
{
	vinfo fbinfo;
	ioctl(fd, FBIOGET_VSCREENINFO, &fbinfo);
	switch(SCREEN_USING)
	{
		case V_SCREEN0:fbinfo.yoffset = yres;	break;
		case V_SCREEN1:fbinfo.yoffset = 0;	break;
		default:break;
	}

//	ioctl(fd, FBIOPUT_VSCREENINFO, &fbinfo);
	ioctl(fd, FBIOPAN_DISPLAY, &fbinfo);
	SCREEN_USING = !SCREEN_USING;
//	printf("yoffset=%u, screen%d\n", fbinfo.yoffset, SCREEN_USING);
}

void make_color(color_8 *out, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	out->red = red;
	out->green = green;
	out->blue = blue;
	out->alpha = alpha;
}

void draw_point(unsigned char *buff, uint32_t x, uint32_t y, color_8 color)
{
	//设置画点内存空间指针位置
	uint32_t offset = (y * xres + x) * bits_per_pixel / BIT_TO_BYTE;
	//由于实际设备单个像素占用4字节，故可以一次性写入单个像素提高效率
	*(uint32_t *)(buff + offset) = (color.alpha << alpha.offset) | (color.red << red.offset) | (color.green << green.offset) | (color.blue << blue.offset);

	// *(unsigned char *)(buff + offset + alpha.offset / BIT_TO_BYTE) = color.alpha;
	// *(unsigned char *)(buff + offset + red.offset / BIT_TO_BYTE) = color.red;
	// *(unsigned char *)(buff + offset + green.offset / BIT_TO_BYTE) = color.green;
	// *(unsigned char *)(buff + offset + blue.offset / BIT_TO_BYTE) = color.blue;
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
	draw_func(buff, func, xmin, xmax, color, 0, 0);
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

void draw_sblock(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color)
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

	for (int j = ymin; j < ymax; j++)
	{
		for (int i = xmin; i < xmax; i++)
			draw_point(buff, i, j, color);
		usleep(10000);
	}
}

void draw_vblock(unsigned char* buff, int xmin, int xmax, int ymin, int ymax, color_8 color,  enum ScreenNum screen)
{
	ymin += screen * yres;
	ymax += screen * yres;
	draw_block(buff, xmin, xmax, ymin, ymax, color);
}

void draw_flash_block(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color, int steps, int delay)
{
	int i;
	static color_8 black;
	make_color(&black, 0, 0, 0, 0);
	draw_vblock(buff, xmin, xmax, ymin, ymax, color, SCREEN_USING);
	if (xmin == 0 && steps >= 1)
	{
		draw_vblock(buff, xmin + i, xmax + i, ymin, ymax, color, V_SCREEN1 - SCREEN_USING);
		switch_vscreen();
		usleep(delay);
		for(i = 2; i < steps; i++)
		{
//			wipe_oldscreen(buff);
			draw_vblock(buff, xmin + i - 2, xmax + i - 2, ymin, ymax, black, V_SCREEN1 - SCREEN_USING);
			draw_vblock(buff, xmin + i, xmax + i, ymin, ymax, color, V_SCREEN1 - SCREEN_USING);
			switch_vscreen();
			usleep(delay);
		}
	}
	else
	{
		for(i = 1; i < steps; i++)
		{
//			wipe_oldscreen(buff);
			draw_vblock(buff, xmin + i - 2, xmax + i - 2, ymin, ymax, black, V_SCREEN1 - SCREEN_USING);
			draw_vblock(buff, xmin + i, xmax + i, ymin, ymax, color, V_SCREEN1 - SCREEN_USING);
			switch_vscreen();
			usleep(delay);
		}
	}

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

void draw_vBMP(unsigned char* buff, char path[], uint32_t xbias, uint32_t ybias, enum ScreenNum screen)
{
	printf("%s\n", path);
	uint32_t ybias0 = ybias + screen * yres;
	draw_BMP(buff, path, xbias, ybias0);
}

void draw_BMP(unsigned char* buff, char path[], uint32_t xbias, uint32_t ybias)
{
	FILE *fp;
	if (fp = fopen(path, "r"))
	{
		BMP_head head;
		BMP_info info;

		if (1 != fread(&head, sizeof(BMP_head), 1, fp))
		{
			return;
		}
		if (1 != fread(&info, sizeof(BMP_info), 1, fp))
		{
			return;
		}
		if(fseek(fp, head.f_Offset, SEEK_SET))
		{
			return;
		}

		printf("BitCount = %d\n", info.BitCount);
		printf("Height = %d, Width = %d\n", info.Height, info.Width);
		switch(info.BitCount)
		{
			case 24:draw_BMP_24(buff, fp, &info, xbias, ybias);	break;
			case 32:draw_BMP_32(buff, fp, &info, xbias, ybias);	break;
			default:break;
		}

		fclose(fp);
	}
}


void draw_BMP_24(unsigned char *buff, FILE *fp, BMP_info *info, uint32_t xbias, uint32_t ybias)
{
	int x = 0;
	int y = 0;
	int width_offset;
	color_8 color;
	width_offset = info->Width * 3 / 4 * 4 + 4 - info->Width * 3;
	//printf("offset = %d\n", width_offset);
	if (info->Height > 0)
	{
		//FIX y = 0 wrong
		for (y = info->Height + ybias; y > ybias; y--)
		{
			for(x = xbias; x < info->Width + xbias; x++)
			{
				fread(&color, 3, 1, fp);
				//fread(&(color.blue), 1, 1, fp);
				//fread(&(color.green), 1, 1, fp);
				//fread(&(color.red), 1, 1, fp);
				color.alpha = 0;
				draw_point(buff, x, y - 1, color);
			}
			fseek(fp, width_offset, SEEK_CUR);
		}
	}
	else if(info->Height < 0)
	{
		int tmpheight = 0 - info->Height;
		for (y = ybias; y < tmpheight + ybias; y++)
		{
			for(x = xbias; x < info->Width + xbias; x++)
			{
				fread(&color, 3, 1, fp);
				color.alpha = 0;
				draw_point(buff, x, y, color);
			}
			fseek(fp, width_offset, SEEK_CUR);
		}
	}
}



void draw_BMP_32(unsigned char *buff, FILE *fp, BMP_info *info, uint32_t xbias, uint32_t ybias)
{
	int x = 0;
	int y = 0;
	color_8 color;

	//printf("Height = %d, Width = %d\n", info->Height, info->Width);
	if (info->Height > 0)
	{
		for (y = info->Height + ybias; y > ybias; y--)
		{
			for(x = xbias; x < info->Width + xbias; x++)
			{
				fread(&color, 4, 1, fp);
				draw_point(buff, x, y - 1, color);
			}
		}
	}
	else if(info->Height < 0)
	{
		int tmpheight = 0 - info->Height;
		for (y = ybias; y < tmpheight + ybias; y++)
		{
			for(x = xbias; x < info->Width + xbias; x++)
			{
				fread(&color, 4, 1, fp);
				draw_point(buff, x, y, color);
			}
		}
	}
}

void draw_flash_BMP(unsigned char *buff, char *path[], uint32_t xbias[], uint32_t ybias[], uint32_t frames, unsigned int sec)
{
	printf("%s\n%s\n" , path[0], path[1]);
	uint32_t i;
	draw_vBMP(buff, path[0], xbias[0], ybias[0], SCREEN_USING);
	printf("screen%d\n",SCREEN_USING);
	sleep(sec);
	for(i = 1; i < frames; i++)
	{
		draw_vBMP(buff, path[i], xbias[i], ybias[i], V_SCREEN1 - SCREEN_USING);
		switch_vscreen();
		printf("screen%d\n",SCREEN_USING);
		sleep(sec);
	}
}

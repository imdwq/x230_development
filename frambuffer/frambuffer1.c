#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "frambuffer.h"

color red, green, blue, alpha;

unsigned int	xres;
unsigned int	yres;
unsigned int	bits_per_pixel;
unsigned int	screensize;

int main(int argc, char *argv[])
{
	vinfo fbinfo;
	int fd = open ("/dev/fb", O_RDWR);

	ioctl(fd, FBIOGET_VSCREENINFO, &fbinfo);
	init(&fbinfo);
	unsigned char *fbp = 0;

	fbp = (unsigned char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	unsigned int offset = (5 * xres + 10) * bits_per_pixel / 8;
	printf("xres=%u redoffset=%u, redlength=%u, offset=%u screensize=%u xres=%u", xres , red.offset, red.length, offset, screensize, fbinfo.xres);

	//draw_block(fbp, 100, 200, 100, 200, 255, 0, 0);
	//draw_block(fbp, 100, 200, 200, 300, 0, 255, 0);
	//draw_block(fbp, 100, 200, 300, 400, 0, 0, 255);
}

void init(vinfo *fbinfo)
{
	xres = fbinfo -> xres;
	yres = fbinfo -> yres;
	bits_per_pixel = fbinfo -> bits_per_pixel;
	screensize = xres * yres;

	red.offset = fbinfo -> red.offset;
	red.length = fbinfo -> red.length;
	blue.offset = fbinfo -> blue.offset;
	blue.length = fbinfo -> blue.length;
	green.offset = fbinfo -> green.offset;
	green.length = fbinfo -> green.length;
	alpha.offset = fbinfo -> transp.offset;
	alpha.length = fbinfo -> transp.length;
}

void draw_point(unsigned char *buff, int x, int y,unsigned int red_p, unsigned int green_p, unsigned int blue_p)
{
	unsigned int offset = (y * xres + x) * bits_per_pixel / 8;
	*(unsigned char *)(buff + offset + red.offset) = red_p;
	*(unsigned char *)(buff + offset + green.offset) = green_p;
	*(unsigned char *)(buff + offset + blue.offset) = blue_p;

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

//frambuffer.h
#ifndef _FRAMBUFFER_
#define _FRAMBUFFER_

#include <stdio.h>
#include <stdint.h>
#include <linux/fb.h>

enum DrawFunction{line, cos, sin};
enum ScreenNum{V_SCREEN0, V_SCREEN1};

typedef struct fb_var_screeninfo vinfo;
typedef struct fb_bitfield fbcolor;
typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
}color_8;

#pragma pack(1)
typedef struct
{
	char f_Type[2];
	uint32_t f_Size;
	uint16_t f_Reserved_1;
	uint16_t f_Reserved_2;
	uint32_t f_Offset;

}BMP_head;

typedef struct
{
	uint32_t Size;
	int32_t Width;
	int32_t Height;
	int16_t Planes;
	int16_t BitCount;
	uint32_t Compression;
	uint32_t SizeImage;
}BMP_info;
#pragma pack()

typedef int DrawFunc;
typedef DrawFunc (*IntFunc)(int);

void make_color(color_8 *out, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

void draw_point(unsigned char *buff, uint32_t x, uint32_t y, color_8 color);
void draw_func(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color, int x0, int y0);
void draw_func_0(unsigned char *buff, IntFunc func, int xmin, int xmax, color_8 color);
void draw_line(unsigned char *buff, int xmin, int xmax, int ymin, int ymax, color_8 color);
void draw_block(unsigned char *buff, int xmin,int xmax, int ymin, int ymax, color_8 color);
void draw_vblock(unsigned char *buff, int xmin,int xmax, int ymin, int ymax, color_8 color, enum ScreenNum screen);
void draw_flash_block(unsigned char *buff, int xmin,int xmax, int ymin, int ymax, color_8 color, int steps, int delay);

unsigned char *fb_init();
void release_fb(unsigned char *buff);
void wipe_screen(unsigned char *buff);
void wipe_oldscreen(unsigned char *buff);
void wipe_allscreen(unsigned char *buff);
void switch_vscreen();

int y_axis_trans(int y);
void zero_func_trans(int *x, int *y, int x0, int y0);

DrawFunc func_line(int x);
DrawFunc func_parabola(int x);

void draw_BMP(unsigned char *buff, char path[], uint32_t xbias, uint32_t ybias); 
void draw_vBMP(unsigned char *buff, char path[], uint32_t xbias, uint32_t ybias, enum ScreenNum screen); 
void draw_BMP_24(unsigned char *buff, FILE *fp, BMP_info *info, uint32_t xbias, uint32_t ybias);
void draw_BMP_32(unsigned char *buff, FILE *fp, BMP_info *info, uint32_t xbias, uint32_t ybias);
void draw_flash_BMP(unsigned char *buff, char *path[], uint32_t xbias[], uint32_t ybias[], uint32_t frames, unsigned int sec); 

#endif

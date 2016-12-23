/*
 * fbconfig.h
 *
 * Headers for utility routines for framebuffer interaction
 *
 */

#ifndef _FBCONFIG_H
#define _FBCONFIG_H
#include <sys/types.h>
#include <asm/types.h>

/* This constant, being ORed with the color index tells the library
 * to draw in exclusive-or mode (that is, drawing the same second time
 * in the same place will remove the element leaving the background intact).
 */
#define XORMODE	0x80000000

extern __u32 xres, yres;

int open_framebuffer(void);
void close_framebuffer(void);
void setcolor(unsigned colidx, unsigned value);
void put_cross(int x, int y, unsigned colidx);
void put_string(int x, int y, char *s, unsigned colidx);
void put_string_center(int x, int y, char *s, unsigned colidx);
void pixel (int x, int y, unsigned colidx);
void line (int x1, int y1, int x2, int y2, unsigned colidx);
void rect (int x1, int y1, int x2, int y2, unsigned colidx);
void fillrect (int x1, int y1, int x2, int y2, unsigned colidx);

/*
 * New functions and struct place here.
 */

#include <stdio.h>

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
	__u32 f_Size;
	__u16 f_Reserved_1;
	__u16 f_Reserved_2;
	__u32 f_Offset;

}BMP_head;

typedef struct
{
	__u32 Size;
	__s32 Width;
	__s32 Height;
	__s16 Planes;
	__s16 BitCount;
	__u32 Compression;
	__u32 SizeImage;
}BMP_info;
#pragma pack()

void draw_point(int x, int y, color_8 color);
void make_color(color_8 *out, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void draw_BMP(char path[], __u32 xbias, __u32 ybias);
void draw_BMP_24(FILE *fp, BMP_info *info, __u32 xbias, __u32 ybias);
void draw_BMP_32(FILE *fp, BMP_info *info, __u32 xbias, __u32 ybias);


#endif /* _FBCONFIG_H */

#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();

	wipe_allscreen(fbp);
	draw_BMP(fbp, "1.bmp", 50, 50);
	wipe_allscreen(fbp);
	draw_BMP(fbp, "2.bmp", 50, 50);
	wipe_allscreen(fbp);
	draw_BMP(fbp, "1234.bmp", 50, 50);
	wipe_allscreen(fbp);
	draw_BMP(fbp, "1.bmp", 50, 50);
	wipe_allscreen(fbp);
	draw_BMP(fbp, "2.bmp", 50, 50);

	release_fb(fbp);

	return 0;
}

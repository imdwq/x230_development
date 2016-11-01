#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = 0;
	fbp = fb_init();

	wipe_screen(fbp);

	draw_BMP(fbp, "1234.bmp");

	release_fb(fbp);

	return 0;
}

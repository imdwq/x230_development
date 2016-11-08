#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = 0;
	int fd = 0;

	fd = fb_init(&fbp);

	wipe_screen(fbp);
	draw_BMP(fbp, "1234.bmp", 50, 50);
	release_fb(fbp);

	return 0;
}

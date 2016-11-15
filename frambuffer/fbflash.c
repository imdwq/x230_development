#include "frambuffer.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();

	uint32_t bias[]={10, 10, 10, 10, 10};
	char *path[] = {"1.bmp", "2.bmp", "1234.bmp", "1.bmp", "2.bmp"};
	
	wipe_allscreen(fbp);
	draw_flash_BMP(fbp, path, bias, bias, 5, 0);

	release_fb(fbp);

	return 0;
}

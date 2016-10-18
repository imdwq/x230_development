#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = 0;
	fbp = fb_init();

	wipe_screen(fbp);

//	draw_block(fbp, 100, 200, 100, 200, 255, 0, 0);
//	draw_block(fbp, 100, 200, 200, 300, 0, 255, 0);
//	draw_block(fbp, 100, 200, 300, 400, 0, 0, 255);
	
	draw_block(fbp, 100, 200, 300, 400, 255, 0, 0);
	draw_block(fbp, 100, 200, 200, 300, 0, 255, 0);
	draw_block(fbp, 100, 200, 100, 200, 0, 0, 255);

	release_fb(fbp);

	return 0;
}

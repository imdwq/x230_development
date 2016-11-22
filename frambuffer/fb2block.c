#define FB_DEBUG 1
#include "frambuffer.h"
#include <unistd.h>

#define DELAY 8000



int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();
	PDEBUG("fb0init\n");
	unsigned char *fbp1 = fb1_init();
	PDEBUG("fb1init\n");
	int temp;
	color_8 color;

	wipe_allscreen(fbp);
	PDEBUG("fb0clean\n");
	wipe_screen0(fbp1);
	PDEBUG("fb1clean\n");

	make_color(&color, 0, 255, 0, 0);
	draw_block(fbp, 100, 400, 0, 200, color);
	make_color(&color, 255, 0, 0, 255);
	draw_block(fbp1, 100, 400, 0, 100, color);


	sleep(2);
	release_fb(fbp);
	release_fb1(fbp1);

	return 0;
}

#define FB_DEBUG 0
#include "frambuffer.h"
#include <unistd.h>

#define DELAY 8000



int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();
	PDEBUG("fb0init\n");
	unsigned char *fbp1 = fb1_init();
	PDEBUG("fb1init\n");
	unsigned char *fbp2 = fb2_init();
	int temp;
	color_8 color;

	wipe_allscreen(fbp);
	PDEBUG("fb0clean\n");
	wipe_allscreen(fbp1);
	PDEBUG("fb1clean\n");
	wipe_allscreen(fbp2);

	make_color(&color, 255, 0, 0, 255);
	draw_sblock(fbp, 0, 800, 0, 480, color);
	make_color(&color, 0, 0, 255, 127);
	draw_sblock(fbp1, 0, 800, 0, 480, color);
	make_color(&color, 255, 0, 255, 63);
	draw_sblock(fbp2, 0, 800, 0, 480, color);
	make_color(&color, 0, 0, 0, 255);
	draw_sblock(fbp, 0, 800, 0, 480, color);
	make_color(&color, 255, 0, 0, 255);
	draw_sblock(fbp, 0, 800, 0, 480, color);
	make_color(&color, 255, 255, 255, 63);
	draw_sblock(fbp2, 0, 800, 0, 480, color);

	
	make_color(&color, 0, 0, 0, 0);
	draw_sblock(fbp, 0, 800, 0, 480, color);
	draw_sblock(fbp1, 0, 800, 0, 480, color);
	draw_sblock(fbp2, 0, 800, 0, 480, color);

	make_color(&color, 255, 0, 255, 255);
	draw_sblock(fbp, 0, 800, 0, 480, color);
	make_color(&color, 255, 255, 255, 127);
	draw_sblock(fbp1, 0, 800, 0, 480, color);
	make_color(&color, 255, 255, 255, 63);
	draw_sblock(fbp2, 0, 800, 0, 480, color);

	getchar();	
	release_fb(fbp);
	release_fb(fbp1);
	release_fb(fbp2);

	return 0;
}

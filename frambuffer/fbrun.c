#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();

	wipe_allscreen(fbp);

//	draw_block(fbp, 100, 200, 100, 200, 255, 0, 0);
//	draw_block(fbp, 100, 200, 200, 300, 0, 255, 0);
//	draw_block(fbp, 100, 200, 300, 400, 0, 0, 255);

	color_8 red, green, blue;
	make_color(&red, 255, 0, 0, 0);
	make_color(&green, 0, 255, 0, 0);
	make_color(&blue, 0, 0, 255, 0);


//	draw_block(fbp, 100, 200, 300, 400, red);
//	draw_block(fbp, 100, 200, 200, 300, green);
//	draw_block(fbp, 100, 200, 100, 200, blue);

	draw_line(fbp, 100, 600, 100, 300, green);

	draw_func(fbp, func_line, 0, 200, red, 0, 0);
	draw_func(fbp, func_parabola, 0, 10, blue, 200, 200);

	release_fb(fbp);

	return 0;
}

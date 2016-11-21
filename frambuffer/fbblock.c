#include "frambuffer.h"
#include <unistd.h>

#define DELAY 8000

int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();
	int temp;

	color_8 color;
	make_color(&color, 0, 255, 0, 0);

	wipe_allscreen(fbp);

	draw_flash_block(fbp, 100, 200, 100, 200, color, 500, DELAY);
	sleep(1);

	wipe_allscreen(fbp);

	switch_vscreen();

	for(temp = 100; temp < 600; temp++)
	{
		draw_block(fbp, temp, temp + 100, 100, 200, color);
		usleep(DELAY);
		wipe_allscreen(fbp);
	}

	return 0;
}

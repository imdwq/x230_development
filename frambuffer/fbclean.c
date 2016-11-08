#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = 0;
	int fd;
	fd = fb_init(&fbp);

	wipe_screen(fbp);
	release_fb(fbp);

	return 0;
}

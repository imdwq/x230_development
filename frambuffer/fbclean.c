#include "frambuffer.h"

int main(int argc, char *argv[])
{
	unsigned char *fbp = fb_init();

	wipe_allscreen(fbp);
	release_fb(fbp);

	return 0;
}

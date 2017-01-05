#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "GPIO.h"


int main(int argc, char *argv[])
{
	int fd = open("/xled", O_RDWR);

	gpio_info gpio;

	int err = ioctl(fd, GPIOGET, &gpio);
	char c = gpio.Data;
	if (err != 0)
		printf("Err = %d\n", err);
	else
		printf("GPJ0 = 0x%x\n", c);
	sleep(2);

	c = ~((1 << 3) | (1 << 4) | (1 << 5));
	gpio.Data = c;
	gpio.Ctrl = 0x00111000;
	err = ioctl (fd, GPIOSET, &gpio);
	if (err != 0)
		printf("Err = %d\n", err);
	else
		printf("xled light on\n");
	sleep(2);

	err = ioctl(fd, XLEDBLANK, 0);
	if (err != 0)
		printf("Err = %d\n", err);
	else
		printf("xled blank\n");

	close(fd);
	return 0;
}

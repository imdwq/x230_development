#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd = open("/xled", O_RDWR);
	char c  = (1 << 3 | 1 << 4);
	printf("write = 0x%x\n", c);
	write(fd, &c, 1);

	sleep(2);

	c  = (1 << 5);
	printf("write = 0x%x\n", c);
	write(fd, &c, 1);
	close(fd);
	return 0;
}

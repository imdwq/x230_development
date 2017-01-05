#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd = open("/xled", O_RDWR);
	char c;
	read(fd, &c, 1);
	printf("read = 0x%x\n", c);

	sleep(2);

	read(fd, &c, 1);
	printf("read = 0x%x\n", c);

/*
	c = ~c; 
	write(fd, &c, 1);
	printf("write = 0x%x\n", c);
	read(fd, &c, 1);
	printf("read = 0x%x\n", c);

	sleep(2);

	c = ~c; 
	write(fd, &c, 1);
	printf("write = 0x%x\n", c);
	read(fd, &c, 1);
	printf("read = 0x%x\n", c);
*/
	close(fd);
	return 0;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "GPIO.h"

#define SETBIT(n) (1 << n)

//keymap of hardware
//LEFT  = GPH0[2]
//DOWN  = GPH0[3]
//UP    = GPH2[0]
//RIGHT = GPH2[1]
//BACK  = GPH2[2]
//MENU  = GPH2[3]

#define LEFTBUTTON  SETBIT(4)
#define DOWNBUTTON  SETBIT(5)
#define UPBUTTON    SETBIT(0)
#define RIGHTBUTTON SETBIT(1)
#define BACKBUTTON  SETBIT(2)
#define MENUBUTTON  SETBIT(3)

#define LED_NUM 3 

enum Buttons {UP, LEFT, DOWN, RIGHT, BACK, MENU};

void lightled(int fd, int state);
int keyscan(int fd);

int main(int argc, char *argv[])
{
	int fdled = open("/xled", O_RDWR);
	int fdkey = open("/xkeypad", O_RDWR);
	int key;
	int led_state = 0;
	int light_sign = LED_NUM;
	ioctl(fdled, XLEDBLANK, 0);
	while(1)
	{
		key = keyscan(fdkey);
		if((key == MENU))
			break;
		else
		{
			switch(key)
			{
				case UP:
					light_sign = LED_NUM;
					lightled(fdled, led_state);
					break;
				case LEFT:
					led_state = ((led_state - 2 + LED_NUM) % LED_NUM) + 1;
					printf("state=%d\n",led_state);
					lightled(fdled, led_state & light_sign);
					break;
				case DOWN:
					light_sign = 0;
					lightled(fdled, 0);
					break;
				case RIGHT:
					led_state = ((led_state + LED_NUM) % LED_NUM) + 1;
					printf("state=%d\n",led_state);
					lightled(fdled, led_state & light_sign);
					break;
				case BACK:
					lightled(fdled, LED_NUM + 1);
				default:break;	
			}
		}	
	}

/*
	c = ~c; 
	write(fd, &c, 1);
	printf("write = 0x%x\n", c);
	read(fd, &c, 1);
	printf("read = 0x%x\n", c);

	sleep(2);

*/
	ioctl(fdled, XLEDBLANK, 0);
	close(fdkey);
	close(fdled);
	return 0;
}

void lightled(int fd, int state)
{
	char led = 0x38;
	if(state == LED_NUM + 1)
		write(fd, &led, 1);
	else if(state == 0)
		ioctl(fd, XLEDBLANK, 0);
	else
	{
		printf("state=%d\n",state);
		led = SETBIT(state + 2);
		write(fd, &led, 1);
	}
}

int keyscan(int fd)
{
	int ret = -1;
	static int key = -1;
	static int key_state = 0;               
	char key_press;
        read(fd, &key_press, 1);
	switch(key_state)
	{
		case 0:
			if(key_press) key_state = 1;
			usleep(10000);
			break;
		case 1:
			key_state = 2;
			switch(key_press)
			{
				case 0:
					key_state = 0;
					break;
				case UPBUTTON:
					key = UP;
					break;
				case RIGHTBUTTON:
					key = RIGHT;
					break;
				case BACKBUTTON:
					key = BACK;
					break;
				case MENUBUTTON:
					key = MENU;
					break;
				case LEFTBUTTON:
					key = LEFT;
					break;
				case DOWNBUTTON:
					key = DOWN;
					break;
				default:
					key_state = 0;
					break;
			}
		case 2:
			if(!key_press)
			{
				key_state = 0;
				ret = key;
				key = -1;
			}
			break;
	}
	return ret;
}

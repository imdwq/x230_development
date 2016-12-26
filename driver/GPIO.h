#ifndef GPIO_H
#define GPIO_H
#define GPIO_IOC 233

#define GPIOSET _IOW(GPIO_IOC, 0, int)
#define GPIOGET _IOR(GPIO_IOC, 0, int)
#define GPIORW _IOWR(GPIO_IOC, 0, int)
#define XLEDBLANK _IOW(GPIO_IOC, 1, int)

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244
#define GPJ0PUD 0xE0200248
#define GPJ0DRV 0xE020024C
#define GPJ0CONPDN 0xE0200250
#define GPJ0PUDPDN 0xE0200254
#define GPJ1CON 0xE0200260
#define GPJ1DAT 0xE0200264
#define GPJ1PUD 0xE0200268
#define GPJ1DRV 0xE020026C
#define GPJ1CONPDN 0xE0200270
#define GPJ1PUDPDN 0xE0200274
#define GPJ2CON 0xE0200280
#define GPJ2DAT 0xE0200284
#define GPJ2PUD 0xE0200288
#define GPH2CON 0xE0200C40
#define GPH2DAT 0xE0200C44
#define GPH2PUD 0xE0200C48
#define GPH0CON 0xE0200C00
#define GPH0DAT 0xE0200C04
#define GPH0PUD 0xE0200C08

#define ALLIOIN 0x00000000
#define ALLIOOUT 0x11111111 

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



typedef struct gpio_struct
{
	volatile int *pCtrl[4];
	volatile int *pData[4];
	volatile int *pPut[4];
}gpio_t;

typedef struct gpio_struct_info
{
	int Ctrl;
	int Data;
	int Put;
}gpio_info;

#endif /* end of include guard: GPIO_H */

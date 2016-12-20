#ifndef GPIO_H
#define GPIO_H
#define GPIO_IOC 233

#define GPIOSET _IOW(GPIO_IOC, 0, int)
#define GPIOGET _IOR(GPIO_IOC, 0, int)
#define GPIORW _IOWR(GPIO_IOC, 0, int)
#define XLEDBLANK _IOW(GPIO_IOC, 1, int)

typedef struct gpio_struct
{
	volatile int *pCtrl;
	volatile int *pData;
	volatile int *pPut;
}gpio_t;

typedef struct gpio_struct_info
{
	int Ctrl;
	int Data;
	int Put;
}gpio_info;

#endif /* end of include guard: GPIO_H */

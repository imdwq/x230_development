#include "xkeypad.h"
#include "GPIO.h"

static dev_t major;

int init_module(void)
{
	if(!(major = register_chrdev(0, "xkeypad", &xkeypad_fop)))
	{
		printk("unable to get device number!\n");
		return -1;
	}

	printk("xkeypad module installed, major device number is %u\n", major);
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(major, "xkeypad");
	printk("xkeypad module removed\n");
}


int xkeypad_open(struct inode *inode, struct file *filp)
{
	gpio_t *gpio;
	filp->private_data = kmalloc(sizeof(gpio_t), GFP_KERNEL);
	gpio = (gpio_t *)(filp->private_data);
	gpio->pCtrl = ioremap(GPJ2CON, 4);
	gpio->pData = ioremap(GPJ2DAT, 4);
	gpio->pPut = ioremap(GPJ2PUD, 4);

	*(gpio->pCtrl) = ALLIOIN;

	*(gpio->pPut) = (1 << 1) | (1 << 3) | (1 << 5) | (1 << 7) | (1 << 9) | (1 << 11) | (1 << 13) | (1 << 15);

	try_module_get(THIS_MODULE);
	return 0;
}
int xkeypad_release(struct inode *inode, struct file *filp)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	iounmap(gpio->pCtrl);
	iounmap(gpio->pData);
	iounmap(gpio->pPut);
	kfree(gpio);
	

	module_put(THIS_MODULE);
	return 0;
}

ssize_t xkeypad_read(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	char c;

//	*(gpio->pCtrl) = XLEDIO;
//	c = (char)*(gpio->pData);
//	c &= ~(XLEDSIGN);

//	*(gpio->pCtrl) = ALLIOIN;
//	*(gpio->pPut) = (2 << 0) | (2 << 1) | (2 << 2) | (2 << 3) | (2 << 4) | (2 << 5) | (2 << 6) | (2 << 7);

	c = (char)*(gpio->pData);

	copy_to_user(buff, &c, 1);
	return 1;
}

ssize_t xkeypad_write(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	char c,tmp;
	copy_from_user(&c, buff, 1);
//	c = ~c;
//	c &= (XLEDSIGN);

//	*(gpio->pCtrl) = XLEDIO;
//	tmp = *(gpio->pData);
//	tmp &= ~(XLEDSIGN);
//	*(gpio->pData) = tmp;
//	*(gpio->pData) |= c;

//	*(gpio->pCtrl) = ALLIOOUT;
//	*(gpio->pData) = ~c;
	return 1;
}

long xkeypad_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	gpio_t *gpio_p = (gpio_t *)filp->private_data;
	gpio_info gpio;
	gpio.Data = *(gpio_p->pData);
	gpio.Ctrl = *(gpio_p->pCtrl);
	int err = 0;
	switch (cmd)
	{
		case GPIOSET:
			err = !access_ok(VERIFY_READ, (void *)arg, sizeof(gpio)); 
			if (err) 
				return -EFAULT;
			else
			{
				copy_from_user(&gpio, (void *)arg, sizeof(gpio));
				*(gpio_p->pCtrl) = gpio.Ctrl;
				*(gpio_p->pData) = gpio.Data;
			}
			break;

		case GPIOGET:
			err = !access_ok(VERIFY_WRITE, (void *)arg, sizeof(gpio)); 
			if (err) 
				return -EFAULT;
			else 
				copy_to_user((void *)arg, &gpio, sizeof(gpio));
			break;

		case XLEDBLANK:
			*(gpio_p->pCtrl) = XLEDIO;
			*(gpio_p->pData) |= XLEDSIGN; 
			break;
		default:
			return -ENOTTY;
			break;
	}
	return 0; 
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("xKEYPAD DRIVER");
MODULE_AUTHOR("141180014");

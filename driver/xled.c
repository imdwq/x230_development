#include "xled.h"
#include "GPIO.h"

static dev_t major;

int init_module(void)
{
	if(!(major = register_chrdev(0, "xled", &xled_fop)))
	{
		printk("unable to get device number!\n");
		return -1;
	}

	printk("xled module installed, major device number is %u\n", major);
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(major, "xled");
	printk("xled module removed\n");
}


int xled_open(struct inode *inode, struct file *filp)
{
	gpio_t *gpio;
	filp->private_data = kmalloc(sizeof(gpio_t), GFP_KERNEL);
	gpio = (gpio_t *)(filp->private_data);
	gpio->pCtrl = ioremap(GPJ0CON, 4);
	gpio->pData = ioremap(GPJ0DAT, 4);

	try_module_get(THIS_MODULE);
	return 0;
}
int xled_release(struct inode *inode, struct file *filp)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	iounmap(gpio->pCtrl);
	iounmap(gpio->pData);
	kfree(gpio);
	

	module_put(THIS_MODULE);
	return 0;
}

ssize_t xled_read(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	char c;
	*(gpio->pCtrl) = ALLIOIN;
	c = ~(char)*(gpio->pData);
	copy_to_user(buff, &c, 1);
	return 1;
}

ssize_t xled_write(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	char c;
	copy_from_user(&c, buff, 1);
	*(gpio->pCtrl) = ALLIOOUT;
	*(gpio->pData) = ~c;
	return 1;
}

long xled_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	int err = 0;
	switch (cmd)
	{
		case GPIOSET:
			err = !access_ok(VERIFY_READ, (void *)arg, sizeof(gpio)); 
			if (err) 
				return -EFAULT;
			else 
				copy_from_user(gpio, (void *)arg, sizeof(gpio));
			break;

		case GPIOGET:
			err = !access_ok(VERIFY_WRITE, (void *)arg, sizeof(gpio)); 
			if (err) 
				return -EFAULT;
			else 
				copy_to_user((void *)arg, gpio, sizeof(gpio));
			break;

		default:
			return -ENOTTY;
			break;
	}
	return 0; 
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("xLED DRIVER");
MODULE_AUTHOR("141180014");

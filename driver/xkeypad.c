#include "xkeypad.h"

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
//	gpio->pCtrl = ioremap(GPJ2CON, 4);
//	gpio->pData = ioremap(GPJ2DAT, 4);
//	gpio->pPut = ioremap(GPJ2PUD, 4);
	gpio->pCtrl[0] = ioremap(GPH0CON, 4);
	gpio->pData[0] = ioremap(GPH0DAT, 4);
	gpio->pPut[0] = ioremap(GPH0PUD, 4);
	gpio->pCtrl[2] = ioremap(GPH2CON, 4);
	gpio->pData[2] = ioremap(GPH2DAT, 4);
	gpio->pPut[2] = ioremap(GPH2PUD, 4);
	*(gpio->pCtrl[0]) = ALLIOIN;
	*(gpio->pPut[0]) = KEYPADH0PUT; 
	*(gpio->pCtrl[2]) = ALLIOIN;
	*(gpio->pPut[2]) = KEYPADH2PUT; 
	try_module_get(THIS_MODULE);
	return 0;
}
int xkeypad_release(struct inode *inode, struct file *filp)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	iounmap(gpio->pCtrl[0]);
	iounmap(gpio->pData[0]);
	iounmap(gpio->pPut[0]);
	iounmap(gpio->pCtrl[2]);
	iounmap(gpio->pData[2]);
	iounmap(gpio->pPut[2]);
	kfree(gpio);
	

	module_put(THIS_MODULE);
	return 0;
}

ssize_t xkeypad_read(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)filp->private_data;
	char c, keyh0, keyh2;
	
	keyh0 = ~(char)*(gpio->pData[0]) & KEYPADH0_MASK;
	keyh2 = ~(char)*(gpio->pData[2]) & KEYPADH2_MASK;
	c = (keyh0 << 2) | keyh2;

	copy_to_user(buff, &c, 1);
	return 1;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("xKEYPAD DRIVER");
MODULE_AUTHOR("141180014");

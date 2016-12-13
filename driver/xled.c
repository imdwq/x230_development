#include "xled.h"

#include <linux/fs.h>



static dev_t major;

int init_module(void)
{
	if(!(major = register_chrdev(0, "xled", &xled_fop)))
	{
		printk("unable to get device number!\n");
		return -1;
	}
	;

	printk("xled module installed\n");
}

void cleanup_module(void)
{

	unregister_chrdev(dev, "xled");
	printk("xled module removed\n");
}


int xled_open(struct inode *inode, struct file *flip)
{
	gpio_t *gpio;
	flip->private_data = kmalloc(sizeof(gpio_t), GFP_KERNEL);
	gpio = (gpio_t *)(flip->private_data);
	unsigned int minor =
	gpio->pCtrl = ioremap(GPJ0CON, 4);
	gpio->pData = ioremap(GPJ0DAT, 4);

	return 0;
}
int xled_release(struct inode *inode, struct file *flip)
{
	gpio_t *gpio = (gpio_t *)flip->private_data;
	gpio->pCtrl = ioremap(GPJ0CON, 4);
	gpio->pData = ioremap(GPJ0DAT, 4);
	kfree(gpio);

	return 0;
}

ssize_t xled_read(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	gpio_t *gpio = (gpio_t *)flip->private_data;
	char c;

	copy_to_user(, buff, count)


}

ssize_t xled_write(struct file *filp, char *buff, size_t count, loff_t *f_pos)
{
	copy_from_user(, buff, count)
}

long xled_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	gpio_t *gpio = (gpio_t *)flip->private_data;
	char val;
	switch (cmd)
	{
		case GPIOSET:


		case GPIOGET:
	}
}


MODULE_DESCRIPTION("xLED DRIVER");
MODULE_AUTHOR("141180014");

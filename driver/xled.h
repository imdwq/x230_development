#ifndef XLED_H
#define XLED_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <asm/uaccess.h>

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244
#define GPJ0PUD 0xE0200248
#define GPJ0DRV 0xE020024C
#define GPJ0CONPDN 0xE0200250
#define GPJ0PUDPDN 0xE0200254

#define ALLIOIN 0
#define ALLIOOUT 0x11111111; 

typedef struct gpio_struct
{
	volatile int *pCtrl;
	volatile int *pData;
}gpio_t;

int xled_open(struct inode *inode, struct file *filp);
int xled_release(struct inode *inode, struct file *filp);
ssize_t xled_read(struct file *filp, char *buff, size_t count, loff_t *f_pos);
ssize_t xled_write(struct file *filp, char *buff, size_t count, loff_t *f_pos);
long xled_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

struct file_operations xled_fop =
{
	.owner = THIS_MODULE,
	.open = xled_open,
	.release = xled_release,
	.read = xled_read,
	.write = xled_write,
	.compat_ioctl = xled_ioctl
};

#endif

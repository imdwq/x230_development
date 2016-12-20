#ifndef XKEYPAD_H
#define XKEYPAD_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <asm/uaccess.h>

#define GPJ1CON 0xE0200260
#define GPJ1DAT 0xE0200264
#define GPJ1PUD 0xE0200268
#define GPJ1DRV 0xE020026C
#define GPJ1CONPDN 0xE0200270
#define GPJ1PUDPDN 0xE0200274
#define GPJ2CON 0xE0200280
#define GPJ2DAT 0xE0200284
#define GPJ2PUD 0xE0200288

#define XLEDIO 0x00111000;
#define XLEDSIGN (1 << 3) | (1 << 4) | (1 << 5)

#define ALLIOIN 0
#define ALLIOOUT 0x11111111; 


int xkeypad_open(struct inode *inode, struct file *filp);
int xkeypad_release(struct inode *inode, struct file *filp);
ssize_t xkeypad_read(struct file *filp, char *buff, size_t count, loff_t *f_pos);
ssize_t xkeypad_write(struct file *filp, char *buff, size_t count, loff_t *f_pos);
long xkeypad_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

struct file_operations xkeypad_fop =
{
	.owner = THIS_MODULE,
	.open = xkeypad_open,
	.release = xkeypad_release,
	.read = xkeypad_read,
	.write = xkeypad_write,
	.unlocked_ioctl = xkeypad_ioctl
};

#endif

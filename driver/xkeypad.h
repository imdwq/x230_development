#ifndef XKEYPAD_H
#define XKEYPAD_H

#include "GPIO.h"

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <asm/uaccess.h>

#define KEYPADFUNCIO 0x00000333
#define KEYPADH0PUT SETBIT(5) | SETBIT(7)
#define KEYPADH2PUT SETBIT(1) | SETBIT(3) | SETBIT(5) | SETBIT(7) 

#define KEYPADH0_MASK 0x0C
#define KEYPADH2_MASK 0x0F


int xkeypad_open(struct inode *inode, struct file *filp);
int xkeypad_release(struct inode *inode, struct file *filp);
ssize_t xkeypad_read(struct file *filp, char *buff, size_t count, loff_t *f_pos);

struct file_operations xkeypad_fop =
{
	.owner = THIS_MODULE,
	.open = xkeypad_open,
	.release = xkeypad_release,
	.read = xkeypad_read,
};

#endif

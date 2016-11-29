#ifndef XLED_H
#define XLED_H

#include <linux/types.h>

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244
#define GPJ0PUD 0xE0200248
#define GPJ0DRV 0xE020024C
#define GPJ0CONPDN 0xE0200250
#define GPJ0PUDPDN 0xE0200254

#define BYTE0 (<<)

struct file_operations *xled_op
{
	
}


int xled_open(struct inode *inode, struct file *flip);
int xled_release(struct inode *inode, struct file *flip);
ssize_t xled_read(struct file *filp, char *buff, size_t count, loff_t *f_pos);
ssize_t xled_write(struct file *filp, char *buff, size_t count, loff_t *f_pos);

#endif

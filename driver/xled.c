#include "xled.h"

#include <linux/fs.h>

volatile static int *pCtrl;
volatile static int *pCtrl

int init_module(void)
{
	register_chrdev(0, "xled");
	volatile int *

}

void cleanup_module(void)
{


}


int xled_open(struct inode *inode, struct file *flip)
int xled_release(struct inode *inode, struct file *flip)

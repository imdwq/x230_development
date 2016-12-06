#include "xled.h"

#include <linux/fs.h>

volatile static int *pCtrl;
volatile static int *pData;

static dev_t major;

int init_module(void)
{
	struct file_ogp

	major = register_chrdev(0, "xled", &xled_fop);

	pCtrl = ioremap(GPJ0CON, 4);
	pData = ioremap(GPJ0DAT, 4);

}

void cleanup_module(void)
{

	unregister_chrdev(dev, "xled");

}


int xled_open(struct inode *inode, struct file *flip)
{
	int num = NUM(inode->i_rdev);
	int type = TYPE(inode->i_rdev);
	flip->private_data = 	
}
int xled_release(struct inode *inode, struct file *flip)

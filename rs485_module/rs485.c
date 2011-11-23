#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#include <linux/moduleparam.h>
#include <linux/major.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/signal.h>
#include <asm/io.h>
/*this header for copy_form_user and copy_to_user*/
#include <asm/uaccess.h>

#include "rs485.h"


#define GPIONAME 	"rs485"
#define GPIO_MAJOR 	122
#define GPIO_MINOR 	0
#define GPIO_BUFFSIZE 	1024

#define GPCCON 0x7f008040
#define GPCDAT 0x7f008044

static unsigned volatile int *gpccon;
static unsigned volatile int *gpcdat;


static int rs485_gpio_set(u32 level)
{
	int tmp;
	tmp = inl((u32)gpcdat);
	
	if(level == 0)
	{
		tmp &= ~(1<<4);
	}
	else if(level == 1)
	{
		tmp |= 1<<4; 
	}
	else
	{
		printk(KERN_INFO "Wrong param Use 0 or 1 to set the level\n");
		return -1;
	}
	outl(tmp,(u32)gpcdat);
	printk(KERN_INFO "set level success\n");
	return 0;
}

static int rs485_gpio_read(u32* buf)
{
	*buf = inl((u32)gpcdat);
	return 0;
}

static int rs485_open(struct inode *inode,struct file *filep)
{
	printk(KERN_INFO "rs485 driver is opened\n");
	return 0;
}

static int rs485_release(struct inode *inode,struct file *filep)
{
	printk(KERN_INFO "rs485 driver is closed\n");
	return 0;
}

static int rs485_ioctl(struct inode *inode,struct file *filep,unsigned int cmd,unsigned long *ioparam)
{
	u32 *p_rs485_dbuf = 0;
	u32 p_rs485_dtmp  = 0;
	u32 cmd_tmp = 0;

	if(!(p_rs485_dbuf = kmalloc(GPIO_BUFFSIZE,GFP_KERNEL)))
	{
		kfree(p_rs485_dbuf);
		return 0;
	}
	memset(p_rs485_dbuf,0,GPIO_BUFFSIZE);

	p_rs485_dtmp = (u32)p_rs485_dbuf;
	cmd_tmp = (u32)cmd;

	switch(cmd_tmp)
	{
		case RS485_SET_GPIO_CMD:
			if(copy_from_user(p_rs485_dbuf,ioparam,4))
			{
				kfree((u32*)p_rs485_dtmp);
				return -1;
			}
			rs485_gpio_set((*p_rs485_dbuf)&0x1);
		break;
		case RS485_READ_GPIO_CMD:
			rs485_gpio_read(p_rs485_dbuf);
			if(copy_to_user(ioparam,p_rs485_dbuf,4))
			{
				kfree((u32*)p_rs485_dtmp);
				return -1;
			}
		break;
		default:
			kfree((u32*)p_rs485_dtmp);
		break;

	}
	kfree((u32*)p_rs485_dtmp);
	return 0;
}




static struct file_operations rs485_fops = {
		.owner	 = THIS_MODULE,
		.ioctl	 = rs485_ioctl,
		.open	 = rs485_open,
		.release = rs485_release,
};


static int rs485_set_dir(u32 direction)
{
	unsigned int  tmp;
	tmp = inl((u32)gpccon);
	tmp &= ~(0xf0000);
	tmp |= 0x10000;
	outl(tmp,(u32)gpccon);
	tmp = inl((u32)gpcdat);
	tmp &= ~(1<<4); 
	outl(tmp,(u32)gpcdat);
	return 0;
}

static int __init rs485_init(void)
{
	int ret = 0;
	printk(KERN_INFO "rs485 :initializing \n");

	gpccon = ioremap(GPCCON,4);
	gpcdat = ioremap(GPCDAT,4);

	if((ret = register_chrdev(GPIO_MAJOR,GPIONAME,&rs485_fops)) < 0)
	{
		printk(KERN_INFO "Top6410 rs485 driver register failed\n");
		return ret;
	}
	rs485_set_dir(1);

	return 0;
}

static void __exit rs485_exit(void)
{
	iounmap(gpccon);
	iounmap(gpcdat);
	unregister_chrdev(GPIO_MAJOR,GPIONAME);
	return ;
}

module_init(rs485_init);
module_exit(rs485_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("ted");
MODULE_DESCRIPTION("This is a test for serial RS485\n");
MODULE_VERSION("ted# version 0.0.1");

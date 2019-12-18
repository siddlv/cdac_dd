#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/circ_buf.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

#define NAME Cirbuff
#define SIZE 16

//Function Prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *indoe, struct file *filp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t  *offp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp);

//Structure that defines the operations that the driver provides
struct file_operations fops =
{
	.owner   = THIS_MODULE,
	.open    = NAME_open,
	.read    = NAME_read,
	.write   = NAME_write,
	.release = NAME_release
};

//Structure for a character driver
struct cdev *my_cdev;
struct circ_buf cbuf;
dev_t Mydev;
int MAJOR, MINOR;

//Init Module
static int __init cirbuff_init(void)
{
	int result;

	result = alloc_chrdev_region(&Mydev, 0, 1, "Cirbuff");
	if (result < 0) {
		printk(KERN_WARNING "\nMyCharDevice: Can't get major device\n");
		return result;
	}

	result = register_chrdev_region(0, 1, "Cirbuff"); //register device region.....

	if (result < 0)
	{
		printk(KERN_ALERT "\nThe region requested is not obtainable\n");
		return (-1);
	}
	printk("\nMajor Number: %d Minor Number: %d\n", MAJOR(Mydev), MINOR(Mydev));
	printk("\nDevice registered\n");

	my_cdev = cdev_alloc();//allocate memory to Char Device structure
	my_cdev->ops = &fops;//link our file operations to the char device

	cbuf.buf = kmalloc(SIZE, GFP_KERNEL);
	if (!cbuf.buf)
	{
		printk("\nError: Buffer not created\n");
		unregister_chrdev_region(Mydev, 1);
		return -ENOMEM;
	}

	result = cdev_add(my_cdev, Mydev, 1); //Notify the kernel abt the new device
	if (result < 0)
	{
		printk(KERN_ALERT "\nChar Device is not created\n");
		kfree(cbuf.buf);
		unregister_chrdev_region(Mydev, 1);
		return (-1);
	}
	return 0;
}

//Cleanup Module
static void __exit cirbuff_exit(void)
{
	printk("\nMajor Number: %d Minor Number: %d\n", MAJOR, MINOR);
	kfree(cbuf.buf);
	unregister_chrdev_region(Mydev, 1); //unregister the device numbers and the device created
	cdev_del(my_cdev);
	printk(KERN_ALERT "\nDevice unregisterd\n");
	return;
}

//Open System Call
int NAME_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\nKernel Open Call\n");
	return 0;
}

//Close System Call
int NAME_release(struct inode *indoe, struct file *filp)
{
	printk(KERN_ALERT "\nKernel Release Call\n");
	return 0;
}

//Write Functionality
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t cnt, loff_t  *offp)
{
	unsigned long result;
	ssize_t retval;
	int i, m;
	m = min(CIRC_SPACE(cbuf.head, cbuf.tail, SIZE), (int) cnt);

	for (i = 0; i < m; i++)
	{
		result = copy_from_user(cbuf.buf + cbuf.head, Ubuff + i, 1); 

		if (0 == result)
		{
			printk(KERN_ALERT "\nUser Message: \n%c\n", cbuf.buf[cbuf.head]);
			retval = cnt;
		}
		else
		{
			printk(KERN_ALERT "\nError Writing Data\n");
			retval = -EFAULT;
			return cnt;
		}
		cbuf.head = (cbuf.head + 1) & (SIZE - 1);
	}
	printk(KERN_ALERT "\nKernel Read Successful\n");
	return i;
}

//Read Functionality
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t cnt, loff_t  *offp)
{
	unsigned long result;
	ssize_t retval;
	int i, m;
	m = min(CIRC_CNT(cbuf.head, cbuf.tail, SIZE), (int) cnt);

	for (i = 0; i < m; i++)
	{
		result = copy_to_user(Ubuff + i, cbuf.buf + cbuf.tail, 1); //get user data
		
		if (0 == result)
		{
			printk(KERN_ALERT "\nKernel Write Succesful\n");
			retval = cnt;
		}
		else
		{
			printk(KERN_ALERT"\nError Reading Data\n");
			retval = -EFAULT;
			return cnt;
		}
		cbuf.tail = (cbuf.tail + 1) & (SIZE - 1);
	}
	return i;
}

//Module over ride functions
module_init(cirbuff_init);
module_exit(cirbuff_exit);

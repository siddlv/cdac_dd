#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

#define NAME MyCharDevice

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

//Init Module
static int __init CharDevice_init(void)
{
	int result;
	int MAJOR,MINOR;
	dev_t Mydev;
	Mydev = MKDEV(255,0);//Create a device number
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nDevice registered\n");
	printk("\nMajor Number: %d Minor Number: %d\n",MAJOR,MINOR);
	result=register_chrdev_region(Mydev,1,"MyCharDevice");//register device region.....
	if(result<0)
	{
		printk(KERN_ALERT "\nThe Region requested for is not obtainable\n");
		return(-1);
	}
	
	my_cdev = cdev_alloc();//allocate memory to Char Device structure
	my_cdev->ops = &fops;//link our file operations to the char device

	result=cdev_add(my_cdev,Mydev,1);//Notify the kernel abt the new device
	if(result<0)
	{
		printk(KERN_ALERT "\nChar Device is not created\n");
		unregister_chrdev_region(Mydev, 1);
		return (-1);
	}
	return 0;
}

//Cleanup Module
void __exit CharDevice_exit(void)
{
	dev_t Mydev;
	int MAJOR,MINOR;
	Mydev=MKDEV(255,0);
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nMajor Number: %d Minor Number: %d\n",MAJOR,MINOR);
	unregister_chrdev_region(Mydev,1);//unregister the device numbers and the device created
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
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[80];
	unsigned long result;
	ssize_t retval;
	//strcpy(Kbuff,Ubuff);
	result=copy_from_user((char *)Kbuff,(char *)Ubuff,count); //get user data
	if(result==0)
	{
		printk(KERN_ALERT "\nUser Message: \n%s\n",Kbuff);
		printk(KERN_ALERT "\nKernel Read Successful\n");	
		retval=count;
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\nError Writing Data\n");
		retval=-EFAULT;
		return retval;
	}
}

//read Functionality	
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp)
{
	char Kbuff[]="Kernel Buffer: Data from Kernel";
	unsigned long result;
	ssize_t retval;
	result=copy_to_user((char *)Ubuff,(char *)Kbuff,sizeof(Kbuff)); //copy to user
	if(result==0)
	{
		printk(KERN_ALERT "\nKernel Write Succesful\n");	
		retval=count;
		return retval;
	}
	else
	{
		printk(KERN_ALERT"\nError Reading Data\n");
		retval=-EFAULT;
		return retval;
	}
}	

//Module over ride functions
module_init(CharDevice_init);
module_exit(CharDevice_exit);

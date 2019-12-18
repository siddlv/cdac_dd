#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/fs.h>

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

dev_t dev_no;
struct cdev c_dev;
struct file_operations fops = {};
int ret;

static int __init chardev_init(void){

	printk(KERN_ALERT "\nIn chardev init function\n");
	
	dev_no = MKDEV(42, 0);
	
	ret = register_chrdev_region(dev_no, 1, "chardev");
	if(ret){
		printk("\nError registering driver\n");
	}
	
	cdev_init(&c_dev, &fops);
	
	ret = cdev_add(&c_dev, dev_no, 1);
	if(ret){
		printk("\nError adding cdev\n");
	}
        
	return 0;
}

static void __exit chardev_exit(void){
        
	printk(KERN_ALERT "\nIn chardev exit function\n");
	
	cdev_del(&c_dev);
	
	unregister_chrdev_region(dev_no, 1);
}

module_init(chardev_init);
module_exit(chardev_exit);



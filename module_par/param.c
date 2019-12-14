#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

static char* charvar = "module";
static int intvar = 10;

module_param(charvar, charp, S_IRUGO);
module_param(intvar, int, S_IRUGO);

static int param_init(void){
	printk(KERN_ALERT "\n We are in init function\n");
	printk(KERN_ALERT "\n The value at charvar is %s\n", charvar);
	printk(KERN_ALERT "\n The value at intvar is %d\n", intvar);
	return 0;
}

static void param_exit(void){
	printk(KERN_ALERT "\n Goodbye\n");
}

module_init(param_init);
module_exit(param_exit);

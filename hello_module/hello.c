#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

static int hello_init(void){
	printk("\nHello World\n");
	return 0;
}

static void hello_exit(void){
	printk("\nGoodbye, World\n");
}

module_init(hello_init);
module_exit(hello_exit);
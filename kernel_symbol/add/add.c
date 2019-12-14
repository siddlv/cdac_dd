#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include"ker_add.h"

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

int add(int a, int b){
	return (a + b);
}

EXPORT_SYMBOL(add);

static int add_init(void){
        printk(KERN_ALERT "\nIn add_mod init function\n");
        return 0;
}

static void add_exit(void){
        printk(KERN_ALERT "\nIn add_mod exit function\n");
}

module_init(add_init);
module_exit(add_exit);

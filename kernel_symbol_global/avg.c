#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "ker_add.h"

MODULE_LICENSE("GPL v3.0");
MODULE_AUTHOR("Siddhesh Dalvi");

static int one = 2;
static int two = 8;
static int avg = 0;

static int avg_init(void){
	printk("\nIn avg_mod init function\n");
	avg = (add(one, two)) / 2;
        printk(KERN_ALERT "\nAverage = %d\n", avg);
        return 0;
}

static void avg_exit(void){
        printk(KERN_ALERT "\nIn avg_mod exit function\n");
}

module_init(avg_init);
module_exit(avg_exit);

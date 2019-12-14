obj-m:=add.o avg.o

KERNELDIR=/lib/modules/$(shell uname -r)/build
#KERNELDIR=/lib/modules/4.19.86-v7l+/build

PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


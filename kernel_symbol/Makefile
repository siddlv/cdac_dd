
obj-y:=add/ avg/

KERNELDIR=/lib/modules/$(shell uname -r)/build
3KERNELDIR=/lib/modules/4.19.86-v7l+/build

PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


ifneq ($(KERNELRELEASE),)
# call from kernel build system

ccflags-y := -std=gnu99 -Wno-declaration-after-statement

nf_mod-objs := netfilter_main.o dns_query.o

obj-m	:= nf_mod.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

install: modules
	sudo insmod nf_mod.ko

reinstall: uninstall install

uninstall: 
	sudo rmmod nf_mod

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers


obj-m += firewall_mod.o

firewall_mod-objs := firewall.o nf_hooks.o node.o parser.o netlink.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f *.o *.ko *.mod.c *.mod *.symvers *.order

ifneq ($(KERNELRELEASE),)
# We were called by kbuild

ifneq ($(strip $(firewall-objs)),)
obj-m += firewall.o
endif

else
# We were called from command line

ifneq ($(strip $(firewall-objs)),)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
else
all: built-in
endif

endif


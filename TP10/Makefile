.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)
obj-m := counter_mutex.o counter_spinlock.o counter_atomic.o counter_per_cpu.o
else
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$$PWD modules

clean:
	make -C $(KERNELDIR) M=$$PWD clean
endif

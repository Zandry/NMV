.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

obj-m := exo06.o

else
	
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc user.c -o user_monitor
	gcc user_request.c -o user_request

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif

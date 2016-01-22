#CC=gcc
#CFLAGS=-g
#CFLAGS=-O3

#all: q1 test

#q1: q1.o
#	${CC} ${CFLAGS} -o q1 q1.o ${LIB}

#q1.o: q1.c
#	${CC} ${CFLAGS} -c q1.c ${LIB}

#test: test.o
#	${CC} ${CFLAGS} -o test test.o ${LIB}

#test.o: test.c
#	${CC} ${CFLAGS} -c test.c ${LIB}

#clean: 
#	rm -f test q1 *~ *.o 

#.PHONY: all 
#.SECONDARY:

.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

obj-m := 

else
	
  KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc test.c -o test

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif

MODULE = xled.o
KMODULE = xled.ko

ifneq($(KERNELRELEASE),)
	obj-m := $(MODULE) 
else
	KDIR := /home/student/dwq/linux-3.0.8
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

endif

CC = arm-linux-gcc
CFLAGS = -o
PROGRAM = fbrun fbclean fbbmp fbflash fbblock fb2block

program : $(PROGRAM) 

fb2block : fb2block.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

fbclean : fbclean.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

libfb.so : frambuffer.o
	$(CC) -shared -o $@ $< 

%.o : %.c
	$(CC) -c  $<



.PHONY : clean cleantmp lib cp

clean :
	rm -f *.o

cleantmp :
	rm -f *.h~ *.c~ Makefile~ .*.un~ .*.*.un~

lib :
	$(CC) -c franbuffer.o
	$(CC) -shared -o $(LIBS) frambuffer.o

cp :
	cp $(PROGRAM) /srv/nfs4/driver
	cp $(KMODULE) /srv/nfs4/driver

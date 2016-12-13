MODULE = xled.o
CC = arm-linux-gcc
PROGRAM = ledread ledwrite
KMODULE = xled.ko

ifneq ($(KERNELRELEASE),)
obj-m	:= $(MODULE) 
else
KDIR	:= /home/student/dwq/linux-3.0.8/
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
endif

program : $(PROGRAM) 

ledread: ledread.o
	$(CC) -o $@ $<

ledwrite: ledwrite.o
	$(CC) -o $@ $<

%.o : %.c
	$(CC) -c  $<


.PHONY : clean cleantmp cp

clean :
	rm -f *.o

cleantmp :
	rm -f *.h~ *.c~ Makefile~ .*.un~ .*.*.un~

cp :
	cp $(PROGRAM) /srv/nfs4/driver
	cp $(KMODULE) /srv/nfs4/driver

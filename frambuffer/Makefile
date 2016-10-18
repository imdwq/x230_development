CC = arm-linux-gcc
CFLAGS = -o

all : fbrun fbclean

fbrun : fbrun.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

fbclean : fbclean.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

libfb.so : frambuffer.o
	$(CC) -shared -o $@ $< 

%.o : %.c
	$(CC) -c  $<



.PHONY : clean

clean :
	rm -f *.o
cleantmp :
	rm -f *.h~ *.c~ Makefile~

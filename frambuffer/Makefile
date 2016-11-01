CC = arm-linux-gcc
CFLAGS = -o
LIBS = libfb.so

all : fbrun fbclean fbbmp

fbbmp : fbbmp.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

fbrun : fbrun.o libfb.so
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
	cp fbrun fbclean fbbmp /srv/nfs4/fb
	cp libfb.so /srv/nfs4/nfslinux/lib

CC = arm-linux-gcc
CFLAGS = -o
LIBS = libfb.so
PROGRAM = fbrun fbclean fbbmp fbflash fbblock fb2block

all : $(PROGRAM) 

fb2block : fb2block.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

fbblock : fbblock.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

fbflash : fbflash.o libfb.so
	$(CC) -o $@ $< -L ./ -l fb

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
	cp $(PROGRAM) /srv/nfs4/fb
	cp $(LIBS) /srv/nfs4/nfslinux/lib

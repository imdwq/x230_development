CC = arm-linux-gcc
PROGRAM = ts_main pop
OBJ = font_8x8.o font_8x16.o fbconfig.o newtest.o

default : $(PROGRAM) 

pop : pop.o $(OBJ)
	$(CC) -o $@ $^ -L ./ libts.so

ts_main : ts_main.o  $(OBJ)
	$(CC) -o $@ $^ -L ./ libts.so
	
%.o : %.c
	$(CC) -c  $<

.PHONY : clean cp cleantmp
cleantmp :
	rm -f *.h~ *.c~ Makefile~ .*.un~ .*.*.un~

clean :
	rm -f *.o

cp :
	cp $(PROGRAM) /srv/nfs4/touchscreen

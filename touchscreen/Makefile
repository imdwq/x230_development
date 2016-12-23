CC = arm-linux-gcc
PROGRAM = ts_main
OBJ = ts_main.o font_8x8.o font_8x16.o fbconfig.o newtest.o

default : $(PROGRAM) 

ts_main : $(OBJ)
	$(CC) -o $@ $^ -L ./ libts.so
	
%.o : %.c
	$(CC) -c  $<

.PHONY : clean cp

clean :
	rm -f *.o

cp :
	cp $(PROGRAM) /srv/nfs4/touchscreen

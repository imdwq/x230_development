CC = arm-linux-gcc

fb : frambuffer.o
	$(CC) -o $@ $<

#fb.o : frambuffer.c
#	$(CC) -c $@ $<

%.o : %.c
	$(CC) -c  $<

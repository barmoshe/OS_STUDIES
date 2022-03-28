CC=gcc
OBJS1=loopcmp.o 
EXEC1=loopcmp
DEBUG = -g
CFLAGS = -std=c99  -Wall -Werror $(DEBUG)

$(EXEC1) : $(OBJS1)
	$(CC) $(OBJS1) -o $@
	
loopcmp.o: loopcmp.c
clean: 
	rm -f $(EXEC) $(OBJS)
	

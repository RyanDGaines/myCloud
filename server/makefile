CC = gcc
OBJS = csapp.o server.o
CFLAGS = -Wall
LFLAGS = -lpthread -o
HEADERS = csapp.h


server: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $@ 

clean:
	rm $(OBJS) server

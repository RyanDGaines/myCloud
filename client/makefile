CC = gcc
OBJS = csapp.o client.o
CFLAGS = -Wall
LFLAGS = -lpthread -o
HEADERS = csapp.h


client: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $@ 
	

clean:
	rm $(OBJS) client

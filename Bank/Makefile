CC= gcc
CFLAGS= -g -Wall -o 
DEPS = simpleList.c 
OBJECTS = simpleList.o
EXECS = client server

all: 
	$(CC) -c simpleList.c

	$(CC) $(CFLAGS) client client.c 
	$(CC) $(CFLAGS) server server.c $(OBJECTS)

clean:
	rm *.o $(EXECS)


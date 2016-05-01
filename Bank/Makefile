CC= gcc
CFLAGS= -g -Wall -o 
DEPS = simpleList.c bank.c
OBJECTS = simpleList.o bank.o -lpthread

EXECS = client server

all: 
	$(CC) -c simpleList.c
	$(CC) -c bank.c -lpthread

	$(CC) $(CFLAGS) client client.c -lpthread
	$(CC) $(CFLAGS) server server.c $(OBJECTS) 

clean:
	rm *.o $(EXECS)


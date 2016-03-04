CC=gcc
CFLAGS= -Wall -g -d
DEPS = mymalloc.c mymalloc.h
OBJECTS = mymalloc.o

all: 
	gcc -c mymalloc.c
	gcc -Wall -g -o test test.c mymalloc.o
clean :
	rm $(OBJECTS)
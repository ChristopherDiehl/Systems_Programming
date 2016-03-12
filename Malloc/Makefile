CC=gcc
CFLAGS= -Wall -g -d
DEPS = mymalloc.c mymalloc.h
OBJECTS = mymalloc.o

all: 
	gcc -c mymalloc.c
	gcc -Wall -g -o test test.c mymalloc.o
	gcc -g -o test1 test1.c mymalloc.o
	gcc -g -o test2 test2.c mymalloc.o
	gcc -g -o test3 test3.c mymalloc.o
	gcc -g -o test4 test4.c mymalloc.o


clean :
	rm $(OBJECTS)


all: sorted-list.h sorted-list.c
	gcc -c sorted-list.c
	ar r libsl.a sorted-list.o
	gcc -Wall -g -o test test.c libsl.a
clean:
	rm *.c

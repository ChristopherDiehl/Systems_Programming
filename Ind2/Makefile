all: sorted-list.h sorted-list.c
	gcc -c sorted-list.c
	ar r libsl.a sorted-list.o
	gcc -Wall -g -o test1 test.c libsl.a
	gcc -Wall -g -o test2 test2.c libsl.a
clean:
	rm *.c

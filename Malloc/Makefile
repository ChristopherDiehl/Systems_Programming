CC= gcc
CFLAGS= -g -o
DEPS = mymalloc.c mymalloc.h
OBJECTS = mymalloc.o
EXECS = test0 test1 test2 test3 test4 test5 test6 test7

all: 
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test0 test0.c $(OBJECTS)
	$(CC) $(CFLAGS) test1 test1.c $(OBJECTS)
	$(CC) $(CFLAGS) test2 test2.c $(OBJECTS)
	$(CC) $(CFLAGS) test3 test3.c $(OBJECTS)
	$(CC) $(CFLAGS) test4 test4.c $(OBJECTS)
	$(CC) $(CFLAGS) test5 test5.c $(OBJECTS)
	$(CC) $(CFLAGS) test6 test6.c $(OBJECTS)
	$(CC) $(CFLAGS) test7 test7.c $(OBJECTS)

test1:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test1 test1.c $(OBJECTS)
test2:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test2 test2.c $(OBJECTS)
test3:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test3 test3.c $(OBJECTS)
test4:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test4 test4.c $(OBJECTS)
test5:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test5 test5.c $(OBJECTS)
test6:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test6 test6.c $(OBJECTS)
test7:
	$(CC) -c mymalloc.c
	$(CC) $(CFLAGS) test7 test7.c $(OBJECTS)
clean :
	rm *.o $(EXECS)


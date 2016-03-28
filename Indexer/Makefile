CC= gcc
CFLAGS= -g -o
DEPS = fileTokenizer.c fileTokenizer.h
OBJECTS = fileTokenizer.o frequencyList.o
EXECS = test

all: 
	$(CC) -c fileTokenizer.c
	$(CC) -c frequencyList.c

	$(CC) $(CFLAGS) test0 tokenizingTest.c $(OBJECTS)
	$(CC) $(CFLAGS) test1 test1.c $(OBJECTS)


clean:
	rm *.o $(EXECS)


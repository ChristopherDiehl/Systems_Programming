CC= gcc
CFLAGS= -g -o
DEPS = fileTokenizer.c fileTokenizer.h
OBJECTS = fileTokenizer.o
EXECS = test

all: 
	$(CC) -c fileTokenizer.c
	$(CC) $(CFLAGS) test0 tokenizingTest.c $(OBJECTS)

clean:
	rm *.o $(EXECS)


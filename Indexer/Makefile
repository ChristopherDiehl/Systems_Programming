CC= gcc
CFLAGS= -g -Wall -o 
DEPS = fileTokenizer.c fileTokenizer.h jsonWriter.h
OBJECTS = fileTokenizer.o frequencyList.o jsonWriter.o
EXECS = test0 test1 test2 test3 test4 test6 index

all: 
	$(CC) -c fileTokenizer.c
	$(CC) -c frequencyList.c
	$(CC) -c jsonWriter.c

	$(CC) $(CFLAGS) index index.c $(OBJECTS)
	$(CC) $(CFLAGS) test0 tokenizingTest.c $(OBJECTS)
	$(CC) $(CFLAGS) test1 test1.c $(OBJECTS)
	$(CC) $(CFLAGS) test2 test2.c $(OBJECTS)
	$(CC) $(CFLAGS) test3 test3.c $(OBJECTS)
	$(CC) $(CFLAGS) test4 test4.c $(OBJECTS)
	$(CC) $(CFLAGS) test6 test6.c $(OBJECTS)


clean:
	rm *.o $(EXECS)


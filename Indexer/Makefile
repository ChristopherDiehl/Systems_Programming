CC = gcc
OBJECTS = main.o sorted-list.o
CFLAGS = -g -c 
OUTPUT=sl
LIB=libsa
all: $(OUTPUT) $(LIB)

sl: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $<

$(LIB): $(OBJECTS)
	ar crs $(LIB) sorted-list.o

clean:
	rm $(LIB) $(OUTPUT) $(OBJECTS)

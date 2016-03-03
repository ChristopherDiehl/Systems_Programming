#ifndef MY_MALLOC_H
#define MY_MALLOC_H
#include "sorted-list.h"
#include <stdlib.h>
/*
 *Christopher && Sandeep
 * MYMALLOC
 * Smarter Malloc
 * Gives Errors and stuff
 */


/*Macros given to us to use by prof*/
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

/* free is an int which determines if the data the MemEntry is keeping track of has been freed
 * 1 = data is in use. 0 = data is available
 * Struct memEntry_ *next points to next memory struct
 * memAllocated is the amount of memory allocated by the user;
 * int specialCode;
 */
typedef struct memEntry_ {
  struct memEntry_ * next;
  struct memEntry_ * prev;
  int free;
  size_t dataStored;
} MemEntry;

/*static variables 
 *memAllocated is how far to increment head when you need to add a new entry
 *numOfMallocs keep track of how many memEntries there are 
 *freeMemEntrys keeps track of how many memEntries are freed
 *if == 0 then no need to look for free memEntries with size == size needed by user
*/
static char ALLMEM [5000];
char * head = ALLMEM;
static int numOfMallocs = 0;
static size_t memAllocated;
static size_t freeMemEntries; 
static MemEntry * tail;

/*actual malloc and free functions*/
void * mymalloc(size_t  size, char * file, int line);
void myfree(void * pointerToFree, char * file, int line);
void * lookForFreeMem(size_t  size);
int (*CompareFunct)( void *, void * );
void (*DestructFunct)( void * );
void defragment(MemEntry * construct);
#endif
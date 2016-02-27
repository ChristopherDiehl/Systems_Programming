#ifndef MY_MALLOC_H
#define MY_MALLOC_H
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

/* free is an int which determines if the data the MemStruct is keeping track of has been freed
 * 1 = data is in use. 0 = data is available
 * Struct MemStruct_ *next points to next memory struct
 * memAllocated is the amount of memory allocated by the user;
 * int specialCode;
 */
typedef struct MemStruct_ {
  struct MemStruct_ * next;
  int free;
  int memAllocated;
} MemStruct;

/*actual malloc and free functions*/
void * mymalloc(size_t size, char * file, int line);
void myfree(void * pointerToFree, char * file, int line);
#endif
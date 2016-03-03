#include "mymalloc.h"
#include "sorted-list.h"
/*Defines */
#define MEMENTRYSIZE sizeof(MemEntry)
#define max_size 5000



/*Static Variables*/
static char ALLMEM [5000];
char * head = ALLMEM;
static int numOfMallocs = 0;
static size_t memAllocated;
static size_t freeMemEntries; 
static MemEntry * tail;



void * mymalloc (size_t size,char * file, int line) {

	//checks is user asks for too much space
	if(size > (max_size+MEMENTRYSIZE)|| size <= 0){
		printf("Insufficient space available. Asked for in %s Line %d",file,line);
		return 0;
	}
	//first time malloc called
	if(numOfMallocs ==0) {
		MemEntry * construct =(MemEntry *)head;
		construct->free =1;
		construct->next =0;
		construct->size = size;
		tail = construct;
		numOfMallocs++;
		memAllocated = MEMENTRYSIZE + size;
	} else {
		if( (size + memAllocated + MEMENTRYSIZE) > max_size){
			printf("Insufficient space available. Asked for in %s Line %d",file,line);
			return 0;
		}
		if(freeMemEntries>0){
			void * lookReturn = lookForFreeMem(size);
			if(lookReturn != 0){
				MemEntry * skipAhead= (MemEntry *)lookReturn;
				lookReturn = (char *)lookReturn+(skipAhead->size);
				return (void *) lookReturn;
			} else {
				//this and first memEntry are only time a new MemEntry is made
			     char * endOfData = head+memAllocated;
			     MemEntry * construct = (MemEntry *) endOfData;
			     construct->free =1;
			     construct->size = size;
			     construct->next=0;
			     tail->next = construct;
			     construct->prev = tail;
			     tail = construct;
			     memAllocated += (MEMENTRYSIZE + size);
			     numOfMallocs++;
			}
		}
	}
	void * returnPtr = head+(memAllocated-size);
	return returnPtr;
}


/*FREE
 *First checks if valid pointer
 *By decrementing pointer by one memEntry struct, we should be able to determine if the pointer is viable
 * if free != 1 then something is wrong... because the memEntry struct should equal 0. So print error
 *then checks if in list
 *if in list, but freed, then skipped over
 *returns 0 and prints an error message if something fails
 */

void myfree(void * pointerToFree, char * file, int line) {
	if(pointerToFree == 0){
		printf("Attempted to free a null pointer\n");
		return;
	}
	if ( pointerToFree < (void *) head || pointerToFree > (void *)head)
	{
		printf("Attempted to free pointer not allocated by malloc\n");
		return;
	}
	MemEntry * construct = (MemEntry *) pointerToFree -1;
	if(construct->free != 1){
		printf("Invalid pointer\n");
		return;
	}else {
		construct->free =0;
		defragment(construct);
	}

}

/*look for free mem tries to find an already free chunk of memory to place the data into
 *RETURNS THE MemEntry STRUCT
 *THE POINTER NEEDS INCREMENTED TO START OF DATA
 *If no memEntrys found that are the same size as the size requested && memAllocated at capacity
 *look for any memEntrys that are > then size looking for. Return that.
 */
void * lookForFreeMem(size_t  size) {
	MemEntry * memEntry = (MemEntry *)head;
	while(memEntry != 0){
		if((memEntry->free=0) && (memEntry->size == size)) {
			memEntry->free=1;
			return (void *) memEntry;
		}
	}
	if(memAllocated == max_size){
     memEntry = (MemEntry *)head;
     while(memEntry != 0){
	  if((memEntry->free=0) && (memEntry->size > size)) {
			memEntry->free=1;
			return (void *) memEntry;
		}
	 }
	}

	return 0;
}

/*defragment handles fragmentation
 *4 cases
 *1)node->next ==0 then freeing last node, can be deleted
 *2)next node is freed
 *3)prev node is freed
 *4) prev and next node are already freed
 */
void defragment(MemEntry * construct){
	if(construct->next ==0){
		memAllocated -= (MEMENTRYSIZE+construct->size);
		tail= construct->prev;
	} else if(construct->next->free ==0 && construct->prev->free ==0){
		MemEntry * root = construct->prev;
		size_t sizeToAddToRoot = construct->next->size + (2 * MEMENTRYSIZE)+construct->size;
		root->next = construct->next->next;
		root->next->prev = root;
		root->size += sizeToAddToRoot;
	} else if(construct->next->free ==0){
		construct->size += (construct->next->size +MEMENTRYSIZE);
		construct->next = construct->next->next;
		construct->next->prev = construct;
	} else if(construct->prev->free ==0){
		defragment(construct->prev);
	}
}

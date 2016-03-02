#include "mymalloc.h"
#include "sorted-list.h"
/*Defines */
#define MEMENTRYSIZE sizeof(MemEntry)
#define max_size 5000


/*
 *head is a pointer for the memory
 */
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
		construct->memAfterStruct = size;
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
				lookReturn = (char *)lookReturn+(skipAhead->memAfterStruct);
				return (void *) lookReturn;
			} else {
				//this and first memEntry are only time a new MemEntry is made
			     char * endOfData = head+memAllocated;
			     MemEntry * construct = (MemEntry *) endOfData;
			     construct->free =1;
			     construct->next=0;
			     construct->memAfterStruct = size;
			     memAllocated = MEMENTRYSIZE + size;
			     appendToList((MenEntry *)head,construct);
			}
		}
	}
	void * returnPtr = head+(memAllocated-size);
	return returnPtr;
}

/*look for free mem tries to find an already free chunk of memory to place the data into
 *RETURNS THE MemEntry STRUCT
 *THE POINTER NEEDS INCREMENTED TO START OF DATA
 */
void * lookForFreeMem(size_t size) {
	MemEntry * memEntry = (MemEntry *)head;
	while(memEntry != 0){
		if(memEntry->free=0 && memEntry->memAfterStruct == size) {
			return (void *) memEntry;
		}
	}
	return 0;
}

/*book keeps the memEntry next, appends the back
 *At most O(n) iterations
*/
void appendToList(MemEntry * head, MemEntry * nodeToAdd){
	MemEntry * prev;
	while (head != 0){
		head = head->next;
		prev = head;
	}
	prev->next = nodeToAdd;
}
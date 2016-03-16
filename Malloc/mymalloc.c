#include "mymalloc.h"

/*Defines */
#define MEMENTRYSIZE sizeof(MemEntry)
#define max_size 5000

//need to 'slice' mementries if user asked for less than a free one
//check if have option to make one free mementry into 2

/*Static Variables*/
static char ALLMEM [5000];
char * head = ALLMEM;
static size_t memAllocated = 0;
static size_t freeMemEntries; 
static MemEntry * tail;



void * mymalloc (size_t size,char * file, int line) {

	//checks is user asks for too much space
	if(size<=0) {
		printf("Invalid amount of data requested %d\n",file,line);
		return 0;
	} else if(size > (max_size+MEMENTRYSIZE) && freeMemEntries == 0){
		printf("Insufficient space available. Asked for in %s Line %d\n",file,line);
		return 0;
	} else if(freeMemEntries > 0 && (memAllocated+size) >= max_size){
		void * lookReturn = lookForFreeMem(size);
		if(lookReturn != 0){
			MemEntry * skipAhead= (MemEntry *)lookReturn;
			lookReturn = (char *)lookReturn+(skipAhead->size);
			return (void *) lookReturn;
		} else{
			printf("Not enough memory avaialble. Asked for in %s Line %d\n",file,line);
			return 0;
		}
	}
	//printf("Current space: %d\n",memAllocated);
	//printf("Space taken by MemEntry: %d\n",MEMENTRYSIZE);
	//printf("User asking for: %d\n",size);
	//printf("Space taken after storage: %d\n",(size + memAllocated + MEMENTRYSIZE));
	//first time malloc called
	if(memAllocated ==0) {
		MemEntry * construct =(MemEntry *)head;
		construct->free =1;
		construct->next =0;
		construct->prev =0;
		construct->code =CODE;
		construct->size = size;
		tail = construct;
		memAllocated = MEMENTRYSIZE + size;
	} else {
		//printf("Current space: %d\n",memAllocated);
		//printf("Space taken after storage: %d\n",(size + memAllocated + MEMENTRYSIZE));
		if( (size + memAllocated + MEMENTRYSIZE) > max_size){
			printf("Insufficient space available. Asked for in %s Line %d\n",file,line);
			return 0;
		}
		if(freeMemEntries>0){
			//there are free memEntries.. look and see if any fit the bill
			void * lookReturn = lookForFreeMem(size);
			if(lookReturn != 0){
				MemEntry * skipAhead= (MemEntry *)lookReturn;
				lookReturn = (char *)lookReturn+(skipAhead->size);
				return (void *) lookReturn;
			} 
		}
		else {
			//this and first memEntry are only time a new MemEntry is made
		     char * endOfData = head+memAllocated;
		     MemEntry * construct = (MemEntry *) endOfData;
		     construct->free =1;
		     construct->size = size;
		     construct->next=0;
		     construct->code=CODE;
		     tail->next = construct;
		     construct->prev = tail;
		     tail = construct;
		     memAllocated = (memAllocated +MEMENTRYSIZE + size);
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
 *prints an error message if something fails
 */

void myfree(void * pointerToFree, char * file, int line) {
	if(pointerToFree == 0){
		printf("Attempted to free a null pointer in %s Line %d\n",file,line);
		return;
	}
	if ( pointerToFree < (void *) head || pointerToFree > (((void *)head+max_size)+MEMENTRYSIZE))
	{
		printf("Attempted to free pointer not allocated by malloc in %s Line %d\n",file,line);
		return;
	}
	MemEntry * construct = (MemEntry *) pointerToFree -1;
	if(construct->code != CODE){
		printf("Invalid pointer in %s Line %d\n",file,line);
		return;
	}else if(construct->free != 1){
		printf("Trying to free already freed pointer in %s Line %d\n",file,line);
		return;
	}else {
		construct->free =0;
		freeMemEntries++;
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
	MemEntry * memEntry =  tail;
	while(memEntry != 0){
		if((memEntry->free==0) && (memEntry->size == size)) {
			memEntry->free=1;
			freeMemEntries--;
			return (void *) memEntry;
		} else{
			memEntry = memEntry->prev;
		}
	}

	memEntry =  tail;
	while(memEntry != 0){
		if((memEntry->free==0) && (memEntry->size > size)) {
			memEntry->free=1;
			freeMemEntries--;
			//printf("MEMENTRY BEFORE %d\n", memEntry->size);
			slice(memEntry,size);
			//printf("MEMENTRY After %d\n", memEntry->size);

			return (void *) memEntry;
		} else{
			memEntry = memEntry->prev;
		}
	}

	return 0;
}

/*slice is only used when the list is full, and a free node with a size >= to the size of data user requested
 *check if MemEntry has enough size to segment a new MemEntry while still returning a pointer to the size of
 *data user requested. 
 *If the there is < 10 bytes that a new MemEntry would have inside it, then don't slice because it is not worth computations
 *to go through list just for an additional MemEntry with 10 bytes, which wouldnt be asked for often
 *If not enough space then return pointer to more data than user needs
 *IN ORDER TO SLICE, THE NEW MEMENTRY MUST HAVE AT LEAST 10 bytes
 *Returns 1 if sliced
 *Returns 0 if not sliced
 */
 int slice(MemEntry * construct, size_t size) {
 	if(construct->size < (size+MEMENTRYSIZE+10)){
 		return 0;
 	} else {
 		char * newEntry = (char*)construct;
		newEntry = newEntry +(MEMENTRYSIZE+size);
 		MemEntry * newConstruct = (MemEntry *) newEntry;
 		newConstruct->size = (construct->size - size -MEMENTRYSIZE);
		construct->size = size;
 		newConstruct->free = 0;
 		newConstruct->code =CODE;
 		newConstruct->next = construct->next;
 		newConstruct->prev = construct;
 		construct->next = newConstruct;
 		freeMemEntries++;
 		if(newConstruct->next !=0)
 			newConstruct->next->prev = newConstruct;
 		return 1;
 	}
 }


/*defragment handles fragmentation
 *4 cases
 *1)node->next ==0 then freeing last node, can be deleted
 *2)next node is freed
 *3)prev node is freed
 *4) prev and next node are already freed
 * NEED TO DECREASE MEMALLOCATED IF CONSTRUCT->NEXT ==0 BECAUSE END NODE
 */
void defragment(MemEntry * construct){

	if(construct->next != 0 && construct->next->free ==0 && construct->prev != 0 && construct->prev->free == 0) {
		freeMemEntries--;

		construct->size += (construct->next->size+MEMENTRYSIZE);
		construct->next = construct->next->next;
		if(construct->next != 0){ //if == 0 then construct is new tail node and  can be deleted
			construct->next->prev = construct;

		} else {
			freeMemEntries--;
			memAllocated = memAllocated - (construct->size +MEMENTRYSIZE);
			if(construct->prev != 0)
				construct->prev->next = 0;
		}
		construct->size += (construct->prev->size + MEMENTRYSIZE);
		freeMemEntries--;
		construct->prev=construct->prev->prev;
		if(construct->prev != 0) //construct is new head
			construct->prev->next = construct;
	}
	else if (construct->next != 0 && construct->next->free == 0){
		freeMemEntries--;

		construct->size += (construct->next->size + MEMENTRYSIZE);
		construct->next = construct->next->next;
		if(construct->next != 0){
			construct->next->prev = construct;
		} else {
			freeMemEntries--;
			tail = construct->prev;
			memAllocated = memAllocated - (construct->size +MEMENTRYSIZE);
			if(construct->prev != 0)
				construct->prev->next = 0;
		}

	} else if(construct->prev != 0 && construct->prev->free ==0){
		freeMemEntries--;

		construct->size += (construct->prev->size +MEMENTRYSIZE);
		construct->prev = construct->prev->prev;
		if(construct->prev != 0){
			construct->prev->next = construct;
		}
		if(construct->next == 0){ // then construct is tail node and can be deleted
			freeMemEntries--;
			tail = construct->prev;
			memAllocated = memAllocated - (construct->size +MEMENTRYSIZE);
			if(construct->prev != 0)
				construct->prev->next = 0;
		}
	} else if(construct->next ==0 ){
		tail = construct->prev;
		freeMemEntries--;
		if(tail != 0)
			tail->next = 0;
		memAllocated= memAllocated- (construct->size +MEMENTRYSIZE);
	} 
}

void printList (MemEntry * construct)
{
	int i = 0;
	while(construct!=0){
		printf("ENTRY %d SIZE: %d  Free:%d\n", i, construct->size, construct->free );
		construct = construct->next;
		i++;
	}
	printf("END PRINT LIST\n");
}

void printFromTail ()
{

	printf("MemAllocated %d\n",memAllocated);
	printf("freeMemEntries %d\n",freeMemEntries);
	if(tail == 0)
		return;
	MemEntry * construct = tail;
	int i = 0;
	while(construct!=0){
		printf("ENTRY %d SIZE: %d  Free:%d\n", i, construct->size, construct->free );
		construct = construct->prev;
		i++;
	}
	printf("END PRINT LIST\n");
}
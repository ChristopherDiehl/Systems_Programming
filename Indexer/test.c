/*Test.c will check test cases and be used throughout developement */
#include <stdio.h>
#include "sorted-list.h"

int compareint(void * num0, void *num1) {
	int *compare0 = (int*)num0;
	int *compare1 = (int*)num1;
	if(* compare1 > *compare0)
		return 1;
	if (*compare1 < *compare0)
		return -1;
	return 0;
}
void intDestructFunc(void * thingToDestroy){
  free((int* )thingToDestroy);
}
int main(){
  int (*compare)(void*,void*);
  compare = &compareint;
  void (*destruct)(void *);
  destruct = &intDestructFunc;
  SortedListPtr slp = SLCreate(compare,destruct);
  int i = 0;
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  i =1;
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 2;
  }
  CYCLE(slp);
  SLDestroy(slp);
  return 0;
}




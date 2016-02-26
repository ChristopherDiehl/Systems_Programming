#include "sorted-list.h"
#include <stdlib.h>
#include <stdio.h>
int IntCmpFun(void * a , void * b)
{
	int *aInt = (int *) a ; 
	int *bInt = (int *) b ; 
	if(*aInt < *bInt )
		return -1;
	if( *aInt > *bInt)
		return 1; 	
	return 0 ; 
}

void IntDesFun(void * a)
{
	free(a);
}
int main() {
	printf("Starting final test\n");
	SortedListPtr slpt1 = SLCreate(IntCmpFun , IntDesFun);
  int i = 0;
  while(i <= 4) {
    int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 1)
      printf("Insert not working correctly line 37");
    i+= 2;
  }
  i = 1;
  while(i <= 2) {
    int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 1)
      printf("Insert not working correctly line 37");
    i+= 2;
  }
  i = -10;
  while(i <= 2) {
    int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 1)
      printf("Insert not working correctly line 37");
    i+= 2;
  }

SortedListIteratorPtr itrt1 = SLCreateIterator(slpt1);
SortedListIteratorPtr itrt = SLCreateIterator(slpt1);
SLDestroy(slpt1);
printf("Starting iterator test .. \n");
for( i = 298; i >= 0 ; i--)
{
   printf("DATA: %d\n",*(int* )SLGetItem(itrt));
   int * tempt =  (int *)SLNextItem(itrt);
   if(tempt == 0){
	   printf("HERE");
	   break;
   }
  

}

for( i = 298; i >= 0 ; i--)
{
   printf("DATA: %d\n",*(int* )SLGetItem(itrt1));
   int * tempt =  (int *)SLNextItem(itrt1);
   if(tempt == 0){
	  // printf("HERE");
   	   if(SLGetItem(itrt1) ==0){
		printf("WORKING");
		break;  
	}
   }
}
return 0;
}

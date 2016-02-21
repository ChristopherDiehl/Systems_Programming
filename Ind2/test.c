#include "sorted-list.h"
#include <stdio.h>

//typedef int (*CompareFuncT)( void *, void * );
//typedef void (*DestructFuncT)( void * );

int IntCmpFun(void * a , void * b)
{
	int *aInt = (int *) a ; 
	int *bInt = (int *) b ; 
//	printf(" INT A % d",*aInt);
//	printf(" INT B % d",*bInt);
	if(*aInt < *bInt )
		return -1;
	if( *aInt > *bInt)
		return 1; 	
	return 0 ; 
}

void IntDesFun(void * a)
{
  free ((int *)a);
}
 

int main(int argc , char ** argv)
{


SortedListPtr slp = SLCreate(IntCmpFun , IntDesFun);
  int i = 0;
  while(i <= 10) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
  i =0 ;
   while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
 

for( i = 100 ; i > 0 ; i-- )
{
	//SLRemove(slp,(void *)( &i));
}



SortedListIteratorPtr itr = SLCreateIterator(slp);

		
for( i = 0 ; i < 10 ; i++ )
{
	int * temp =  (int *)SLGetItem(itr);
	printf(" HEX VALUES : %p\n",  temp);
	if(temp != NULL)
		printf(" INT VALUES : %d\n",  *temp);
}
i =0 ;
   while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    //int returnVal = SLRemove(slp,(void*)newvalue);
    //printf("REMOVE Return value:  %d\n",returnVal);
    i+= 1;
  }
SortedListIteratorPtr itr2 = SLCreateIterator(slp);
i =0;
for( i = 0 ; i < 10 ; i++ )
{
	int * temp =  (int *)SLGetItem(itr2);
	//printf(" NEXT HEX VALUES : %p\n",  temp);
	if(temp != NULL)
		printf(" INT VALUES : %d\n",  *temp);

}



	SLDestroy(slp);
  int * temp =  (int *)SLGetItem(itr2);
    printf(" INT VALUES : %d\n",  *temp);

	return 0; 
}

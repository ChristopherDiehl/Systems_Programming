
#include "sorted-list.h"
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
}
 

int main(int argc , char ** argv)
{


SortedListPtr slp = SLCreate(IntCmpFun , IntDesFun);
  int i = 0;
  while(i <= 10) {
    int * newvalue = (int *) malloc (sizeof(int ));
   *  newvalue = i;
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
}



SortedListIteratorPtr itr = SLCreateIterator(slp);

    
for( i = 0 ; i < 10 ; i++ )
{
  int * temp =  (int *)SLGetItem(itr);
  //printf(" HEX VALUES : %p\n",  temp);
  if(temp != NULL)
    printf(" INT VALUES : %d\n",  *temp);
}
i =0 ;
   while(i < 10) {
    int * newvalue = (int *) malloc (sizeof(int ));

    printf("Removing: %d I: %d", *newvalue,i);
    * newvalue = i;
    int returnVal = SLRemove(slp,(void*)newvalue);
    printf("Remove COmpleete; Return value:  %d\n",returnVal);
    i+= 1;
  }
  for( i = 0 ; i < 10 ; i++ )
{
  int * temp =  (int *)SLGetItem(itr);
  //printf(" HEX VALUES : %p\n",  temp);
  if(temp != NULL)
    printf(" INT VALUES : %d\n",  *temp);
}

  SLDestroy(slp);

  return 0; 
}

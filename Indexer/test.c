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

printf("Starting first test -test.c-\n");
SortedListPtr slp = SLCreate(IntCmpFun , IntDesFun);
  int i = 0;
  while(i <= 100) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    if(returnVal != 1)
      printf("Insert not working correctly line 37");
    i+= 1;
  }
  //test for duplicate handling
  i =0 ;
   while(i < 100) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    if(returnVal != 0)
      printf("Duplicate not correct line 46\n");
    i+= 1;
  }
 

for( i = 200 ; i < 300 ; i++ )
{
	 int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    if(returnVal != 1)
      printf("Insert not correct line 57\n");
}

for( i = 101; i < 200; i++ )
{
   int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
   int returnVal = SLInsert(slp,(void*)newvalue);

    if(returnVal != 1)
      printf("Insert middle not correct line 66 %d\n",i);
}


SortedListIteratorPtr itr = SLCreateIterator(slp);

printf("Starting iterator test .. \n");
for( i = 298; i >= 0 ; i--)
{

   int * temp =  (int *)SLNextItem(itr);
  

	if(temp != NULL){
      if(i != *temp){
        printf("Error getting list values line 86.\n Expected Return: %d, Return Value: %d\n",i,*temp);
      }
  }

}
SLDestroyIterator(itr);
SortedListIteratorPtr itr2 = SLCreateIterator(slp);
for( i = 298; i >= 0 ; i--)
{
  int * temp =  (int *)SLNextItem(itr2);
  if(temp != NULL){
    if(i != *temp){
      printf("Error getting list values line 96.\n Expected Return: %d, Return Value: %d\n",i,*temp);
    }
  }

}
SortedListIteratorPtr itr3 = SLCreateIterator(slp);
 int * temp =  (int *)SLGetItem(itr3);
  if(*temp != 299)
    printf("Expected 299, recieved %d\n", *temp);
  void * tempInt = (void *)temp;
  SLRemove(slp,tempInt);
  temp =  (int *)SLGetItem(itr3);
  if(*temp != 299)
    printf("Expected 299, recieved %d\n", *temp);
  temp =  (int *)SLNextItem(itr3);
  if(*temp != 298)
    printf("Expected 299, recieved %d\n", *temp);
  SLDestroyIterator(itr2);
  SLDestroy(slp);
  temp =  (int *)SLGetItem(itr3);
  if(*temp != 298)
    printf("Expected 298, recieved %d\n", *temp);
  printf("Test 1 is succesfull\n");
  SLDestroy(itr3);
	return 0; 
}

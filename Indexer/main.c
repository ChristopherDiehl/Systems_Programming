#include "sorted-list.h"
#include <stdio.h>

//typedef int (*CompareFuncT)( void *, void * );
//typedef void (*DestructFuncT)( void * );


typedef struct testingStruct_{
  char * value;
} testingStruct;

int structCmpFun(void * a , void * b)
{
  testingStruct * test1 = (testingStruct *) a;
  testingStruct * test2 = (testingStruct *) b; 
  if(test1->value[0] > test2->value[0])
    return 1;
  else if(test1->value[0] < test2->value[0])
    return -1;
  return 0;
}

void structDesFun(void * a)
{
  testingStruct * test = (testingStruct *) a;
  free(test->value);
  free(test);
}
 

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
 

// TEST 3 struct alacazam 
typedef struct alacazam_
{
	char * valc1 ; // malloced string
	char * valc2 ; 
	int * valNum ; 
	int num ; 	// to compare
} alacazam ; 


alacazam * createAlaz(int num)
{
	alacazam * res = (alacazam *) malloc( sizeof(alacazam)) ;	
	res->valc1 = (char *) malloc( 100 ) ; // 100 bytes 
	res->valc1 = (char *) malloc( 1000 ) ; // 100 bytes 
	res->valNum = (int *) malloc( sizeof(int)) ; 		
	*res->valNum = num ; 

	return res; 
}

void destroyAlaz(void * alazV)
{
	alacazam * alaz = (alacazam *) alazV; 
	free(alaz->valc1);
	free(alaz->valc2);
	free(alaz->valNum);
	free(alaz);
}

int cmpAlaz(void * alazVA , void * alazVB)
{
	alacazam * alazA = (alacazam *) alazVA; 
	alacazam * alazB = (alacazam *) alazVB; 

	if(*alazA->valNum < *alazB->valNum)
		return -1;
	if(*alazA->valNum < *alazB->valNum)
		return 1;
	return 0 ; 
}

int main(int argc , char ** argv)
{

// ALACAZAM TESTING .... 
	printf( "ALACAZAM TESTING" ) ; 

	SortedListPtr alacazamList = SLCreate(cmpAlaz , destroyAlaz);
	int i = 0 ; 
	for( i = 0 ; i < 1000 ; ++i)
	{
		alacazam * tempC = createAlaz(i);
		SLInsert(alacazamList,(void *)tempC);
	}

	for( i = 0 ; i < 1000 ; ++i)
	{
		alacazam * tempCr = createAlaz(i);
		SLInsert(alacazamList,(void *)tempCr);
	}



	SortedListIteratorPtr itrAlcz = SLCreateIterator(alacazamList);

	alacazam * tempAlzc = (alacazam *)SLGetItem(itrAlcz);
	while(tempAlzc != NULL)
	{
		printf(" ALAC %i " , *tempAlzc->valNum);
		tempAlzc = (alacazam *)SLNextItem(itrAlcz);	
	}
	
	SLDestroy(alacazamList);
	
	
//	tempAlzc = (alacazam *)SLNextItem(itrAlcz);	 // points to 
//	printf(" ALAC %i " , *tempAlzc->valNum);


	for( i = 0 ; i < 1000 ; ++i) // wont work at alacazamList is destoryed 
	{
		alacazam * tempCr = createAlaz(i);
		SLInsert(alacazamList,(void *)tempCr);
	}

	alacazamList = SLCreate(cmpAlaz, destroyAlaz);

	for( i = 0 ; i < 10000 ; ++i) // works at alacazam not destoryed
	{
		alacazam * tempCr = createAlaz(i);
		SLInsert(alacazamList,(void *)tempCr);
	}


	itrAlcz = SLCreateIterator(alacazamList);
	
	for( i = 950 ; i < 1005 ; ++i)// remove the first 50 items from list  , but the iterator still points to the first
	{
		alacazam * tempCr = createAlaz(i); // try to remove non existant objects too 
		SLRemove(alacazamList, (void *) tempCr);
	}

		

	for( i = 0 ; i < 50 ; ++i)  // see if the iterator still gives the first 50 items 
	{
	//	tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
//		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLNextItem(itrAlcz);	
		printf(" ALAC %i " , *tempAlzc->valNum);
	}	
	
		tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
		printf(" ALAC %i " , *tempAlzc->valNum);


	printf(" 2 ITERATOR \n" ); 	

	SortedListIteratorPtr itrAlcz2 = SLCreateIterator(alacazamList);

	for( i = 0 ; i < 50 ; ++i)  // see if the iterator still gives the first 50 items 
	{
		tempAlzc = (alacazam *)SLNextItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);

	}	



	printf( "ALACAZAM ENDED " ) ; 

	return 0 ; 

SortedListPtr slp = SLCreate(IntCmpFun , IntDesFun);
   i = 0;
  while(i <= 100) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
  i =0 ;
   while(i < 100) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
 

for( i = 100 ; i > 0 ; i-- )
{
	SLRemove(slp,(void *)( &i));
}



SortedListIteratorPtr itr = SLCreateIterator(slp);

		
for( i = 0 ; i < 100 ; i++ )
{
	int * temp =  (int *)SLGetItem(itr);
	printf(" HEX VALUES : %p",  temp);
	if(temp != NULL)
		printf(" INT VALUES : %d",  *temp);
}

i =0 ;
   while(i < 10000) {
    int * newvalue = (int *) malloc (sizeof(int ));
    * newvalue = i;
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }

SortedListIteratorPtr itr2 = SLCreateIterator(slp);

for( i = 0 ; i < 1000 ; i++ )
{
	int * temp =  (int *)SLNextItem(itr2);
	printf(" NEXT HEX VALUES : %p",  temp);
	if(temp != NULL)
		printf(" INT VALUES : %d",  *temp);

}


SortedListIteratorPtr itr3 = SLCreateIterator(slp);

for( i = 0 ; i < 1000 ; i++ )
{
	printf("%d", *  (int *)SLGetItem(itr3));
}


	SLDestroy(slp);

printf("Test2.c\n");
SortedListPtr slp3 =  SLCreate(structCmpFun , structDesFun);
   i = 0;
  char charArray [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','z'};
  while(i <= 10) {
    testingStruct * newvalue =  (testingStruct *) malloc (sizeof(testingStruct));
    newvalue->value = (char *)malloc(sizeof(char)*2);
    newvalue->value[0] =charArray[i];
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp3,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 51");
    i+= 1;
  }
  while(i <= 20) {
    testingStruct * newvalue =  (testingStruct *)malloc (sizeof(testingStruct));
    newvalue->value = (char *)malloc(sizeof(char)*2);
    newvalue->value[0] = charArray[i];
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp3,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 51");
    returnVal = SLRemove(slp3,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 54");
    i+= 1;
  }
  i=10;
  SortedListIteratorPtr itr4 = SLCreateIterator(slp3);
  testingStruct * temp = (testingStruct *)SLGetItem(itr4);

      
  while(temp !=0)
  {
    if(temp != NULL){
      if(charArray[i] != temp->value[0]){
        printf("Expected %c, got %c\n",charArray[i],temp->value[0]);
      }
      temp = (testingStruct *)SLNextItem(itr4);
      i--;      
    }
  }
  SLDestroyIterator(itr);
  SLDestroy(slp);
  printf("TEST 2 IS SUCCESFULL\n");


	return 0; 
}

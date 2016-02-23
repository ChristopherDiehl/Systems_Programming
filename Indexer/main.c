#include "sorted-list.h"
#include <stdio.h>

// TEST 4 struct 
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
 
// TEST 2 + 3 
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
 

// TEST 1 struct alacazam 
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
	if(*alazA->valNum > *alazB->valNum)
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
	//	printf(" ALAC %i " , *tempAlzc->valNum);
		tempAlzc = (alacazam *)SLNextItem(itrAlcz);	
	}
	
	SLDestroy(alacazamList);
	
	
	tempAlzc = (alacazam *)SLGetItem(itrAlcz);	 // points to 
	//printf(" ALAC %i " , *tempAlzc->valNum);
	

	for( i = 0 ; i < 1000 ; ++i) // wont work at alacazamList is destoryed 
	{
		alacazam * tempCr = createAlaz(i);
		SLInsert(alacazamList,(void *)tempCr);
	}

	SortedListPtr alacazamList2 = SLCreate(cmpAlaz, destroyAlaz);
	
	tempAlzc = (alacazam *)SLGetItem(itrAlcz);	 // points to 
	//printf(" ALAC %i " , *tempAlzc->valNum);
	

	for( i = 0 ; i < 10000 ; ++i) // works at alacazam not destoryed
	{
		alacazam * tempCr = createAlaz(i);
		SLInsert(alacazamList2,(void *)tempCr);
	}


	SortedListIteratorPtr itrAlcz2 = SLCreateIterator(alacazamList2);
	tempAlzc = (alacazam *)SLGetItem(itrAlcz2);	 // points to 
	printf(" ALAC ITERATOR %i \n" , *tempAlzc->valNum);

	tempAlzc = (alacazam *)SLGetItem(itrAlcz);	 // points to 
	printf(" ALAC ITERATOR %i \n" , *tempAlzc->valNum);

	for( i = 9999 ; i > 9950 ; --i)// remove the first 50 items from list  , but the iterator still points to the first
	{
		alacazam * tempCr = createAlaz(i); // try to remove non existant objects too 
		int removed = SLRemove(alacazamList2, (void *) tempCr);
	}

	printf(" ITERATOR 1 \n " ) ; 	

	for( i = 0 ; i < 100 ; ++i)  // see if the iterator still gives the first 50 items 
	{
	//	tempAlzc = (alacazam *)SLGetItem(itrAlcz);	
//		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLNextItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);
	}	
	
		tempAlzc = (alacazam *)SLGetItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);

		tempAlzc = (alacazam *)SLGetItem(itrAlcz2);	
		printf(" ALAC %i " , *tempAlzc->valNum);


	printf(" 2 ITERATOR \n" ); 	
	
	SortedListIteratorPtr itrAlcz3 = SLCreateIterator(alacazamList2);

	for( i = 0 ; i < 50 ; ++i)  // see if the iterator still gives the first 50 items 
	{
		tempAlzc = (alacazam *)SLNextItem(itrAlcz3);	
		printf(" ALAC %i " , *tempAlzc->valNum);
	}	

	SLDestroy(alacazamList);
	SLDestroy(alacazamList2);
	
	SLDestroyIterator(itrAlcz);
	SLDestroyIterator(itrAlcz2);
	SLDestroyIterator(itrAlcz3);

	printf( "ALACAZAM ENDED " ) ; 

// TEST 2 /////////////////////////////////////////


printf("Starting final test\n");
SortedListPtr slpt1 = SLCreate(IntCmpFun , IntDesFun);
  i = 0;
  while(i <= 100) {
    int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 1)
      printf("Insert not working correctly line 37");
    i+= 1;
  }
  //test for duplicate handling
  i =0 ;
   while(i < 100) {
    int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 0)
      printf("Duplicate not correct line 46\n");
    i+= 1;
  }
 

for( i = 200 ; i < 300 ; i++ )
{
   int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
    int returnVal2 = SLInsert(slpt1,(void*)newvalue2);
    if(returnVal2 != 1)
      printf("Insert not correct line 57\n");
}

for( i = 101; i < 200; i++ )
{
   int * newvalue2 = (int *) malloc (sizeof(int ));
    * newvalue2 = i;
   int returnVal2 = SLInsert(slpt1,(void*)newvalue2);

    if(returnVal2 != 1)
      printf("Insert middle not correct line 66 %d\n",i);
}


SortedListIteratorPtr itrt = SLCreateIterator(slpt1);

printf("Starting iterator test .. \n");
for( i = 298; i >= 0 ; i--)
{

   int * tempt =  (int *)SLNextItem(itrt);
  

  if(tempt != NULL){
      if(i != *tempt){
        printf("Error getting list values line 86.\n Expected Return: %d, Return Value: %d\n",i,*tempt);
      }
  }

}
SLDestroyIterator(itrt);
SortedListIteratorPtr itrt2 = SLCreateIterator(slpt1);
for( i = 298; i >= 0 ; i--)
{
  int * tempt =  (int *)SLNextItem(itrt2);
  if(tempt != NULL){
    if(i != *tempt){
      printf("Error getting list values line 96.\n Expected Return: %d, Return Value: %d\n",i,*tempt);
    }
  }

}
SortedListIteratorPtr itrt3 = SLCreateIterator(slpt1);
 int * tempt =  (int *)SLGetItem(itrt3);
  if(*tempt != 299)
    printf("Expected 299, recieved %d\n", *tempt);
  void * temptInt = (void *)tempt;
  SLRemove(slpt1,temptInt);
  tempt =  (int *)SLGetItem(itrt3);
  if(*tempt != 299)
    printf("Expected 299, recieved %d\n", *tempt);
  tempt =  (int *)SLNextItem(itrt3);
  if(*tempt != 298)
    printf("Expected 299, recieved %d\n", *tempt);
  SLDestroyIterator(itrt2);
  SLDestroy(slpt1);
  tempt =  (int *)SLGetItem(itrt3);
  if(*tempt != 298)
    printf("Expected 298, recieved %d\n", *tempt);
  printf("Test is succesfull\n");
  SLDestroy(slpt1);













	












	// TEST 3  Using INTS
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
 

SortedListIteratorPtr itr = SLCreateIterator(slp);
	
for( i = 0 ; i < 100 ; i++ )
{
	int * temp =  (int *)SLNextItem(itr);
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
  if(*(int *)SLGetItem(itr3) != 999)
	 printf("SLGETITEM FAILED %d", *  (int *)SLGetItem(itr3));
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


#include "sorted-list.h"
#include <stdio.h>
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
 

int main(int argc , char ** argv)
{


SortedListPtr slp = SLCreate(structCmpFun , structDesFun);
  int i = 0;
  char charArray [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','z'};
  while(i <= 10) {
    testingStruct * newvalue =  malloc (sizeof(testingStruct));
    newvalue->value = malloc(sizeof(char)*2);
    newvalue->value[0] =charArray[i];
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
  CYCLE(slp);
  while(i <= 20) {
    testingStruct * newvalue =  malloc (sizeof(testingStruct));
    newvalue->value = malloc(sizeof(char)*2);
    newvalue->value[0] = charArray[i];
    printf("Value to be added: %c\n", newvalue->value[0]);
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp,(void*)newvalue);
    printf("Test is complete. Return value:  %d\n",returnVal);
    returnVal = SLRemove(slp,(void*)newvalue);
    printf("Remove is complete. Return value:  %d\n",returnVal);
    i+= 1;
  }
  CYCLE(slp);

  SortedListIteratorPtr itr = SLCreateIterator(slp);
  CYCLE(slp);
  testingStruct * temp = SLGetItem(itr);
  printf(" Value VALUES : %s\n",  temp->value);

      
  while(temp !=0)
  {
     temp =  SLNextItem(itr);
    //printf(" HEX VALUES : %p\n",  temp);
    if(temp != NULL)
      printf(" Value VALUES : %s\n",  temp->value);
  }
  SLDestroyIterator(itr);
  printf("NEW ITERATOR\n");
  //SLDestroy(slp);
  SortedListIteratorPtr itr1 = SLCreateIterator(slp);
  CYCLE(slp);
  SLDestroy(slp);
  printf("STuff is freed");
  temp = SLGetItem(itr1);
  printf(" Value VALUES : %s\n",  temp->value);

      
  /*while(temp !=0)
  {
     temp =  SLNextItem(itr1);
    //printf(" HEX VALUES : %p\n",  temp);
    if(temp != NULL)
      printf(" Value VALUES : %s\n",  temp->value);
  }*/
  //SLDestroy(slp);

  return 0; 
}

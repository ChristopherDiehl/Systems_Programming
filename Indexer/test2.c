
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

printf("Test2.c\n");
SortedListPtr slp = SLCreate(structCmpFun , structDesFun);
  int i = 0;
  char charArray [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','z'};
  while(i <= 10) {
    testingStruct * newvalue =  malloc (sizeof(testingStruct));
    newvalue->value = malloc(sizeof(char)*2);
    newvalue->value[0] =charArray[i];
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 51");
    i+= 1;
  }
  while(i <= 20) {
    testingStruct * newvalue =  malloc (sizeof(testingStruct));
    newvalue->value = malloc(sizeof(char)*2);
    newvalue->value[0] = charArray[i];
    newvalue->value[1]='\0';
    int returnVal = SLInsert(slp,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 51");
    returnVal = SLRemove(slp,(void*)newvalue);
    if(returnVal != 1)
      printf("Test2 failed line 54");
    i+= 1;
  }
  i=10;
  SortedListIteratorPtr itr = SLCreateIterator(slp);
  testingStruct * temp = SLGetItem(itr);

      
  while(temp !=0)
  {
    if(temp != NULL){
      if(charArray[i] != temp->value[0]){
        printf("TEST FAILED LINE 67");
        printf("Expected %c, got %c\n",charArray[i],temp->value[0]);
      }
      temp = SLNextItem(itr);
      i--;      
    }
  }
  SLDestroyIterator(itr);
  SLDestroy(slp);
  printf("TEST 2 IS SUCCESFULL\n");


  return 0; 
}

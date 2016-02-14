/*Test.c will check test cases and be used throughout developement */
#include <stdio.h>
#include "sorted-list.h"

int main(){
  printf("Test is complete");
}

/*compareint is a simple function to be passed for test purposes */
int compareint(void * num, void *num1){
  int *compare0 = (int*)num0;
  int *compare1 = (int*)num1;
  if(compare1>compare0)
	return 1;
  if (compare1<compare0)
	return -1;
  return 0;
}



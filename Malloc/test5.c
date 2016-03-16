#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test for saturation then free middle node and try to fill it with less than size;
#define PTRSIZE 500
int main (int argc, char*argv[]){
	int *ptr;
	free(ptr);
	char * ptr0 = malloc(PTRSIZE);
	char * ptr1 = malloc(PTRSIZE);
	char * ptr2 = malloc(PTRSIZE);
	free(ptr0);
	free(ptr1);
	printf("Freed ptr1 and ptr0\n");
	*ptr2 = 'a';
	printf("%c\n", *ptr2);
	//10 * 500 == 5000.. full  before ptr9 because not enough space


	return 0;
}
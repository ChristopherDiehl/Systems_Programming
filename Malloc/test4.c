#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test for saturation then free middle node and try to fill it with less than size;
#define PTRSIZE 500
int main (int argc, char*argv[]){
	//char * alphabet [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y', 'z'}
	//ask for 5000
	char * ptr0 = malloc(PTRSIZE);
	char * ptr1 = malloc(PTRSIZE);
	char * ptr2 = malloc(PTRSIZE);
	char * ptr3 = malloc(PTRSIZE);
	char * ptr4 = malloc(PTRSIZE);
	char * ptr5 = malloc(PTRSIZE);
	char * ptr6 = malloc(PTRSIZE);
	char * ptr7 = malloc(PTRSIZE);
	char * ptr8 = malloc(PTRSIZE);
	printf("ERROR MESSAGE?:\n");
	char * ptr9 = malloc(PTRSIZE);
	//10 * 500 == 5000.. full  before ptr9 because not enough space
	free(ptr4); //free middle node
	
	printf("FREEING\n");
	char * testForSlice = malloc(PTRSIZE/2);

	//should have a free mementry struct with 218 left
	printf("Shouldn't get an error message\n");
	char * ptrLess = malloc(218);
	printf("DONE\n");

	return 0;
}
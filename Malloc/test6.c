#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test for saturation then free middle node and try to fill it with less than size;
#define PTRSIZE 500
int main (int argc, char*argv[]){
	//char * alphabet [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y', 'z'}
	//ask for 5000
	//check for free defragmentation
	char * ptr0 = malloc(PTRSIZE);
	char * ptr1 = malloc(PTRSIZE);
	char * ptr2 = malloc(PTRSIZE);
	free(ptr0);
	free(ptr1);
	free(ptr2);
	
	//10 * 500 == 5000.. full  before ptr9 because not enough space


	return 0;
}
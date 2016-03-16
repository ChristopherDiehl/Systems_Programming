#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test for saturation then free middle node and try to fill it with less than size;
#define PTRSIZE 500
int main (int argc, char*argv[]){
	//char * alphabet [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y', 'z'}
	//ask for 5000
	//check prof's stuff
	printf("NO ERROR\n");
	char * p = (char *)malloc( 100 ); 
	free( p );
	p = (char *)malloc( 100 ); 
	free( p );
	printf("FIRST PART DONE\n");
	p = (char *)malloc( 200 );
	free( p + 10 );
	free(p);
	free(p);
	int x;
	free(&x);
	//10 * 500 == 5000.. full  before ptr9 because not enough space


	return 0;
}
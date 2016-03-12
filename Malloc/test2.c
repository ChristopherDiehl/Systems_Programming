#include  "mymalloc.h"
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
//test for saturation then free middle node and try to fill it with less than size;
#define PTRSIZE 500
int main (int argc, char*argv[]){
	char  alphabet [] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y', 'z'};
	//ask for 5000
	char * ptr0 = (char *)malloc(PTRSIZE);
	//char * ptr1 = malloc(PTRSIZE);
	int i =0;
	int a =0;
	while(i < PTRSIZE){
		ptr0[i] = alphabet[a];
		if(a >=26){
			a= -1;
		}
		i++;
		a++;
	}
	i = 0;
	a=0;
	while(i <PTRSIZE){
		if(ptr0[i]!=alphabet[a]){
			printf("WHATS HAPPENING\n");
		}
		if(a >=26){
			a= -1;
		}
		printf("%c",ptr0[i] );
		i++;
		a++;
	}
	//10 * 500 == 5000.. full  before ptr9 because not enough space
	
	printFromTail();


	return 0;
}
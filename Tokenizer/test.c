#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int main(int argc, char **argv) {
	char* test= "\"[\",\"]\",\"{\",\"}\",\".\",\"&\",\"*\",\"/\",\"%\",\"+\",\"=\",\";\",\"<\",\">\",\"|\",\"!\",\"~\",\",\",\"+=\",\"-=\",\"*=\",\">>\",\"<<\",\"<=\",\">=\",\"==\",\"!=\",\"&&\",\"||\",\"/=\",\"&=\",\"|=\",\"%=\",\">>=\",\"<<=\",\"^\",\"^=\"";
	int i = 0;
	int count = 0;
	for (i=0; i < strlen(test);i++){
		if(*test=='"'){
			count ++;
		}
	}
	count=count/2;
	printf("Count: %d",count);
}
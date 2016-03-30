
#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"
int main(){
	char * temp = getJsonToken("bacon");
	printf("%s",temp);
	free(temp);
	int anINt = 89;

	char * inter = getFrequency(anINt);
	char * temper = getJsonRecord("bacon.txt",inter,0);
	printf("%s", temper);
	free(temper);

	return 0;

}
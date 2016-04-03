#include "frequencyList.h"
#include "fileTokenizer.h"
#include "jsonWriter.h"
int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=3)
	{
		printf("[-]Please enter valid args\n" );
		return -1 ; 
	}	

	TokenizerT * tk = Tokenize(argv[2]);
	FrequencyList * fList = getFrequencyList();
	while(1)
	{
		char * res = GetToken(tk);
		if(res != 0 && strcmp(res, "") != 0){
			//printf("Trying to add \"%s\" from %s to list\n", res, argv[1]);
			addToList(argv[1], res, fList);

			
		}
		if(res == 0)
			break ; 
		free(res);
		//printf("%s\n",res);

	}	
	jsonWrite(fList,argv[1]);

	deleteList(fList);
	tkDestroy(tk);

  return 0;
}



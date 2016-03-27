#include "fileTokenizer.h"
int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=2)
	{
		return -1 ; 
	}	

	TokenizerT * tk = Tokenize(argv[1]);

	while(1)
	{
		char * res = GetToken(tk);	
		if(res == 0)
			break ; 
		printf("%s\n",res);
		if(res != 0)
			free(res);
	}	

	tkDestroy(tk);
  return 0;
}

#include "fileTokenizer.h"

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 *
 */
/*NEED TO MAKE STRINGS LOWERCASE*/
TokenizerT * Tokenize( char * filename) {
	TokenizerT *res  = (TokenizerT *)malloc(sizeof(TokenizerT)); 
	if( res == 0)
		return 0 ; 

	/*open file*/

	size_t length = file_length(filename) +1;
	FILE * file = fopen(filename,"r");
	if(file == 0){
		printf("[-] Invalid file name passed to tokenizer: %s\n",filename);
		return 0;
	}
	res->_strLen = length;
	char * buffer = malloc(length*sizeof(char));
	if(buffer == 0){
		printf("[-] Malloc for file buffer failed. \n [-] Terminating Program\n");
		return 0;
	}
	fread(buffer, length-1, 1, file);
	// Allocate string space 	
	res->_str = buffer;
	if(res->_str == 0)
		return 0;
	fclose(file);
	//PreprocessesString(res->_str);
	res->_processedLen = 0; 
	res->_state = START;
  return res;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * Frees the tk->_str & the tokenizer struct
 *  - these are the only things malloced besides token which is freed in main
 */

void tkDestroy( TokenizerT * tk ) {
	if(tk != 0){
		if(tk->_str != 0)
			free (tk->_str);
		free (tk);
	}
}
/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char * GetToken( TokenizerT * tk ) {

/* LOOP FORWARD UNTIL WE SEE : 
 *  1) WHITE SPACE 
 *  2) CHANGE FROM ONE STATE TO ANOTHER STATE. 
 */


	char * p = tk->_str + tk->_processedLen ; // points to the last processed item 
	tk->_state = START;
	
	if(*p == '\0')
		return NULL;

	int tokenBeg = tk->_processedLen ;
	
	STATE prevState ;
	while(*p != '\0')
	{
		tk->_state = stateAndCharTest(p,tk);	

		if(tk->_state == INDEF){
				int tokenLen =	tk->_processedLen - tokenBeg ;  
				if(tokenLen == 0)
				{
					++p;	
					++tk->_processedLen; // we disregard this character during the next try
					++tokenBeg; // next token begins after this
					tk->_state = START;

					continue;
				}

				char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
				__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);

				++tk->_processedLen; // we disregard this character during the next try
				if(tokenLen != 0){
					return token;		
				}
		}
		* p = tolower(*p);
		prevState = tk->_state;
		++tk->_processedLen;
		++p;



	}
	int tokenLen =	tk->_processedLen - tokenBeg ;  
	char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
	if (token == 0)
		return NULL;

	__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);	
	//handles if there is no space and tokenLen != 0 IE 132ASF is a digit and a word
	return token;
}






/********************************************************************************* HELPER FUNCTIONS ********************************************************************************************************/



/*Test to see what character is what */
STATE stateAndCharTest(char *p , TokenizerT * tk)
{
	STATE st ;
	switch(tk->_state)
	{
		case START:
			if(isWord(p))
				st = WORD;
			else
				st = INDEF;
			break;
		case WORD:
			if( isWord(p))
				st = WORD;
			else if ( isDigit(p))
				st = WORD;
			else
				st = INDEF;
			break;

		case INDEF:
			st = INDEF;
		default:
			st = INDEF; 
				break ; 
	}
	return st;
}


/* Functions to Check what kind of type the char is */

int isDigit(char *a)
{
	return isdigit(*a);
}

int isSpace(char *a)
{
	return isspace(*a) ;
}

int isWord(char *a)
{
	return isalpha(*a); 
}

int __strncpy(char * dest, char * src , int len)
{
	int i ;
	for(i = 0 ; i < len ; ++i)
	{
		if(*(src+i) ==0){
			break;
		}
		*(dest + i) = *(src + i);
	}
	dest[len] = '\0';	
	return 0 ; 
}

size_t file_length(char * filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

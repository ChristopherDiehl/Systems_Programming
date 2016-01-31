/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/* Define */
#define ARGMISMATCHERROR "Please enter valid arguments \n"
int __strncpy(char * dest, char * src , int len)
{
	int i ;
	for(i = 0 ; i < len ; ++i)
	{
		*(dest + i) = *(src + i);
	}
	dest[len] = '\0';	
	return 0 ; 
}




typedef enum { START ,ZERO ,  OCTAL , HEX , DIGIT , FLOAT , PLUS_OR_MIN ,DOT , EXP, WORD,C_OP ,INDEF } STATE ;

typedef struct TokenizerT_ TokenizerT;
struct TokenizerT_ {
	char * _str; // Points to allocated string copy OR better to use array ? 
	size_t _strLen; // size
	int _processedLen ; // length of string processed
	STATE _state ;  //current state of tokenizer
};



/*
 *
 * Function Prototypes
 *
 *
 */
int isOctal(char *a);
int isDigit(char *a);
int isX(char *a);
int isHex(char *a);
int isWord(char *a);
int isSpace(char *a);
int isDot(char *a);
int isExp(char *a);
int isPlusMinus(char *a);
STATE stateTest(char *p , TokenizerT * tk);
STATE stateAndCharTest(char *p , TokenizerT * tk);
STATE charTest(char *p);
void stateTokenPrint(char * p , TokenizerT *tk );
void PreProcessString(char *ts, size_t ts_length);

/*Preprocesses delims and makes them spaces so main will automatically separate delims into new token*/
void PreProcessString(char *ts, size_t ts_length) {
	int i = 0;
    for(i = 0; i < ts_length; i++){
		switch(ts[i]) {
			case '\t':
				ts[i] = ' ';
			case '\v':
				ts[i] = ' ';
			case '\f':
				ts[i] = ' ';
			case '\n':
				ts[i] = ' ';
			case '\r':
				ts[i] = ' ';
		}
	}
}


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

TokenizerT *TKCreate( char * ts ) {
	TokenizerT *res  = (TokenizerT *)malloc(sizeof(TokenizerT)); 
	if( res == NULL)
		return NULL ; 
	res->_strLen = strlen(ts);
	
	// Allocate string space 	
	res->_str = (char *)malloc(sizeof(char)*res->_strLen);
	if(res->_str == NULL)
		return NULL;
	strcpy(res->_str , ts);
	PreProcessString (res->_str,res->_strLen);
	res->_processedLen = 0 ; 
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

void TKDestroy( TokenizerT * tk ) {
	free (tk->_str);
	free (tk);
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

char *TKGetNextToken( TokenizerT * tk ) {

/* LOOP FORWARD UNTIL WE SEE : 
 *  1) WHITE SPACE 
 *  2) /0 
 *  3)C Comment
 *  4) CHANGE FROM ONE STATE TO ANOTHER STATE. 
 */


	char * p = tk->_str + tk->_processedLen ; // points to the last processed item 
	tk->_state = START;
	
	if(*p == '\0')
		return NULL;

	int tokenBeg = tk->_processedLen ;
	
	STATE prevState ;
	while(*p != '\0')
	{
		if(*p == '/' && *(++p) == '*'){
		    tk->_processedLen += 2;
			tokenBeg += 2;;
			while (*p != '\0'){
				if(*p == '*' && *(++p) == '/') {
					++tk->_processedLen;
					++tokenBeg;
					break;
				}
				++p;
				++tk->_processedLen; 
				++tokenBeg; 
			}
			//have to increment everything again to get past last */
			++p;
		    ++tk->_processedLen; 
			++tokenBeg; 
			continue;
		} 
		//handles C comment //
		else if (*p == '/' && *(++p) == '/') {
			tk->_processedLen += 2;
			tokenBeg += 2;
			while (*p != '\0'){
				if(isSpace(p)) { //space means a new token or end of comment. Lets next if block handle that
					break;
				}
				++p;
				++tk->_processedLen; // we disregard this character during the next try
				++tokenBeg; 
			}
		}
		if(isSpace(p))
		{
			int tokenLen =	tk->_processedLen - tokenBeg ;  
			if(tokenLen == 0)
			{
				++p;	
				++tk->_processedLen; // we disregard this character during the next try
				++tokenBeg; // next token begins after this
				continue;
			}

			char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
			

			__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);	
			++tk->_processedLen; // we disregard this character during the next try

			if(!isSpace(token) && tokenLen != 0)
				return token;		
		} 

	prevState = tk->_state;

	tk->_state = stateAndCharTest(p,tk);	


	if(tk->_state == INDEF)
		break;
	
	++tk->_processedLen;
	++p;
	}
	int tokenLen =	tk->_processedLen - tokenBeg ;  
	char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
	__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);	
	//handles if there is no space and tokenLen != 0 IE 132ASF is a digit and a word
	if(!isSpace(token) && tokenLen != 0 )
	{
		
		if(prevState == 0 && *p != '\0'){
			tk->_state = prevState;
		}
		if(tk->_state == INDEF){
			tk->_state = prevState;
		}
		

	return token;
	}
	else
		return NULL;
}

/*Takes in the current char and the tokenizer
 *Once state is identified, tk->_state is changed to new value
*/


STATE stateAndCharTest(char *p , TokenizerT * tk)
{
	STATE st ;
	switch(tk->_state)
	{
		case ZERO:
		    if(isX(p))
				st = HEX;
			else if(isOctal(p))
				st = OCTAL;
			else 
				st = INDEF;
			break; 
		case C_OP:
			if(ispunct(p))
				st = C_OP;
			else
				st = INDEF;
			break; 
		case OCTAL:
			if(isOctal(p))
				st = OCTAL;
			else
				st = INDEF;
			break; 
		case HEX:
			if(isHex(p))
				st = HEX;				
			else
				st = INDEF;
			break;

		case DOT:
			if(isDigit(p))
				st = FLOAT;
			else if(isExp(p))
				st = FLOAT;
			else
				st = INDEF;
			break;

		case DIGIT:
			if(isDigit(p))	
				st = DIGIT; 
			else if(isDot(p)||isExp(p))
				st = FLOAT;
			else
				st = INDEF;
			break;

		case EXP:
			if(isPlusMinus(p))
				st = PLUS_OR_MIN;
			else if(isDigit(p))
				st = FLOAT;
			else 
				st = INDEF;
			break;
	
		case FLOAT: 
			if(isDigit(p))
				st = FLOAT;
			else if(isExp(p))
				st = EXP;
			else
				st = INDEF;
			break; 

		case PLUS_OR_MIN:
			if(isDigit(p))
				st = FLOAT;
			else
				st = INDEF;
			break;

		case START:
			if ( *p == '0')
				st = ZERO;
			else if(isDigit(p))
				st = DIGIT;
			else if(isWord(p))
				st = WORD;
			else if(isDot(p))
				st = DOT;
			else if(ispunct(*p))
				st = C_OP;
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
		default:
			st = INDEF; // ?? 
				break ; 
	}
	return st;
}

/*Takes in a char* token
 *the token is matched to an entry in tokenArray
 *the index is used to find the corresponding tokenType in tokenType array
 *tokenType is then returned
 *Worst case: O(n) where n is the amount of tokens in tokenArray
*/

char * findC_Op(char * token) {
	//printf("token: %s\n",token);
	char **tokenArray = (char *[]){"[","]","{","}",".","&","*","/","%","+","=",";","<",">","|","!","~",",","+=","-=","*=",">>","<<","<=",">=","==","!=","&&","||","/=","&=","|=","%=",">>=","<<=","^","^=",":"};
	char ** tokenType = (char *[]){"leftbrace", "rightbrace","leftcurlybrace","rightcurlybrace","period","ampersand","asterisk","slash","modulus",	"plus","equals","semicolon","lessthan","greaterthan","vertical bar","exclamation point","tilde","comma","plusequals","minusequals","multiplyequals","shift right","shift left","less or equal","greater or equal","equals","not equals","Logical and","Logical or","divide equals","and equals","or equals","modulo equals","shiftrightequals","shiftleftequals","bitwise exclusive or","exclusive or equals","colon"};
	int tokenArraySize = 38;
	int i = 0;
	for(i = 0; i < tokenArraySize ; i++) {
		if(0 ==strcmp(token,tokenArray[i])){
			return tokenType[i];
		}
	}

	return 0;


}
/*
 * param p : pointer to c string
 * param tk: pointer to tokenizer
 *
 * FUNC : will print state and token to stdout
 * If no state is defined or state is undeterminedthen prints bad token
 * Resets the token state to start state
 */

void stateTokenPrint(char * token , TokenizerT *tk )
{
	if(tk->_state == DIGIT)
		printf("decimal \"%s\"\n",token);
	else if(tk->_state == OCTAL)
		printf("octal \"%s\"\n",token);
	else if(tk->_state == FLOAT)
		printf("float \"%s\"\n",token);
	else if(tk->_state == HEX)
		printf("hex \"%s\"\n",token);
	else if(tk->_state == ZERO)
		printf("zero \"%s\"\n",token);
	else if (tk->_state == WORD)
		printf("word \"%s\"\n",token);
	else if (tk->_state == EXP)
		printf("float \"%s\"\n",token);
	else if(tk->_state == INDEF)
		printf("bad token \"%s\"\n",token);
	else if(tk->_state == DOT)
		printf("%s \"%s\"\n",findC_Op(token),token);
	else if(tk->_state ==C_OP)
		printf("%s \"%s\"\n",findC_Op(token),token);
	else 
		printf("bad token \"%s\"\n",token);

	// resetting the token for the next traversal 
	tk->_state = START; 
}

/* Functions to Check what kind of type the char is */

int isOctal(char *a)
{	
	int b = (int)(*a - '0');
	return b>= 0 && b <= 7 ; 
}

int isDigit(char *a)
{
	return isdigit(*a);
}

int isX(char *a)
{
	return (*a == 'x' || *a == 'X');
}

int isHex(char *a)
{
	return isxdigit(*a);
}

int isSpace(char *a)
{
	return isspace(*a) ;
}

int isWord(char *a)
{
	return (int)(*a) <= 122 && (int)(*a) >= 65 ; 
}

int isDot(char *a)
{
	return (*a=='.');
}

int isPlusMinus(char *a)
{
	return (*a=='+' || *a=='-');
}

int isExp(char *a)
{
	return (*a=='e' || *a=='E');
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 * The token is freed at the end of each while loop iteration
 * Loop only breaks if res == 0 or res is NULL
 */

int main(int argc, char **argv) {

	/* check the input */ 
	if(argc !=2)
	{
		printf(ARGMISMATCHERROR);
		return -1 ; 
	}	

	TokenizerT * tk = TKCreate(argv[1]);
	//printf(" STRING : %s\n", tk->_str);

	while(1)
	{
		char * res = TKGetNextToken(tk);	
		if(res == 0)
			break ; 
		stateTokenPrint(res,tk);
		free(res);
	}	

	TKDestroy(tk);
  return 0;
}
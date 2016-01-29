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


/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

typedef enum { START ,ZERO ,  OCTAL , HEX , DIGIT , FLOAT , PLUS_OR_MIN ,DOT , EXP, WORD,C_OP, LEFT_BRACE , RIGHT_BRACE , INDEF } STATE ;

typedef struct TokenizerT_ TokenizerT;
struct TokenizerT_ {
	char * _str; // Points to allocated string copy OR better to use array ? 
	int _strLen; // size
	int _processedLen ; // length of string processed
	STATE _state ; 
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
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	TokenizerT *res  = (TokenizerT *)malloc(sizeof(TokenizerT)); 
	if( res == NULL)
		return NULL ; 
	res->_strLen = strlen(ts);
	
	// Allocate string space 	
	res->_str = (char *)malloc(sizeof(char)*res->_strLen);
	strcpy(res->_str , ts);
	res->_processedLen = 0 ; 
	res->_state = START;
  return res;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
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
 *  3) CHANGE FROM ONE STATE TO ANOTHER STATE. 
 */


	char * p = tk->_str + tk->_processedLen ; // points to the last processed item 
	tk->_state = START;
	
	if(*p == '\0')
		return NULL;

	int tokenBeg = tk->_processedLen ;  // token begins from this point onwards. 
	
	char * prevChar ;	
	STATE prevState ;
	while(*p != '\0')
	{
		if(isSpace(p))
		{
			int tokenLen =	tk->_processedLen - tokenBeg ;  
			if(tokenLen == 0 || isSpace(prevChar)) // BAD CODE
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

		// STUPID IMPLEMENTATION 
	prevState = tk->_state;

	//	printf("%d",tk->_state);
		tk->_state = stateAndCharTest(p,tk);	


//	printf("%c",*p);
//	printf("%c\n",*(p+1));
	
		if(tk->_state != prevState) // if transisition between two states then we know 
		{
			
//			printf("PREV : %d ",prevState);
//			printf("CURRENT : %d\n",tk->_state);
			if(prevState != START &&prevState != ZERO) // allow transition from start and zero
			{
				if(prevState != DOT && prevState != EXP && prevState!= PLUS_OR_MIN) // allow transition from dot and exp and plusMinus
				{
					if(tk->_state != EXP && tk->_state != DOT && tk->_state != PLUS_OR_MIN) // allow transition to exp and dot and plus minus
					{
					//	printf("STATE CHANGE : TOKEN FORMED\n");
						break ; 
					}
				}
			}
		}
		
		++tk->_processedLen;
		prevChar = p ;
		++p;
	}
	
	int tokenLen =	tk->_processedLen - tokenBeg ;  
	char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
	// COPY FROM STRING TO TOKEN 
	__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);	

	if(!isSpace(token) && tokenLen != 0 )
	{
		if(prevState !=0)
			tk->_state = prevState;

		return token;
	}
	else
		return NULL;
}



STATE stateAndCharTest(char *p , TokenizerT * tk)
{
	STATE st ;
	switch(tk->_state)
	{
		case ZERO:
		    if(isX(p))
				st = HEX;
			else if(isDot(p))
				st = DOT;
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
				st = EXP;
			else
				st = INDEF;
			break;

		case DIGIT:
			if(isDigit(p))	
				st = DIGIT; 
			else if(isDot(p))
				st = DOT;
			else if(isExp(p))
				st = EXP;
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
			{
		//		printf("DIGIT");
				st = DIGIT;
			}
			else if(ispunct(*p)){
				st = C_OP;
			}
			else if(isWord(p))
			{
		//		printf("WORD");
				st = WORD;
			} 
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



char * findC_Op(char * token) {
	printf("token: %s\n",token);
	char **tokenArray = (char *[]){"[","]","{","}",".","&","*","/","%","+","=",";","<",">","|","!","~",",","+=","-=","*=",">>","<<","<=",">=","==","!=","&&","||","/=","&=","|=","%=",">>=","<<=","^","^="};
	char ** tokenType = (char *[]){"leftbrace", "rightbrace","leftcurlybrace","rightcurlybrace","period","ampersand","asterisk","slash","modulus",	"plus","equals","semicolon","lessthan","greaterthan","vertical bar","exclamation point","tilde","comma","plusequals","minusequals","multiplyequals","shift right","shift left","less or equal","greater or equal","equals","not equals","Logical and","Logical or","divide equals","and equals","or equals","modulo equals","shiftrightequals","shiftleftequals","bitwise exclusive or","exclusive or equals"};
	//size_t token_length = strlen(token);
	int tokenArraySize = 37;
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
 * SIDE-EFFECT : none
 *
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
	else if(tk->_state == INDEF)
		printf("mal \"%s\"\n",token);
	else if(tk->_state ==C_OP)
		printf("%s \"%s\"\n",findC_Op(token),token);
	else 
		printf("mal \"%s\"\n",token);

	// resetting the token for the next traversal 
		tk->_state = START; // START OVER AGAIN
}

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
		if(res == NULL)
			break ; 
		stateTokenPrint(res,tk);
		free(res);
	}	

	TKDestroy(tk);
  return 0;
}
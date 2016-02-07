/*
 * tokenizer.c
 *Christopher and Sandeep
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
		if(*(src+i) ==0){
			break;
		}
		*(dest + i) = *(src + i);
	}
	dest[len] = '\0';	
	return 0 ; 
}

/*
 *indef is used to identify the beginning of a new token,
 *if current token is bad then BADTOKEN state is issued ie. 12.0e is a bad token
 *FLOATE is assigned when a float token acquires an e. this stops cases like 1.2e-2e8
*/
typedef enum { START ,ZERO ,  OCTAL , HEX , DIGIT , FLOAT , FLOATE, PLUS_OR_MIN ,DOT , EXP, WORD,C_OP ,INDEF,BADTOKEN,POTENTIALHEX } STATE ;

typedef struct TokenizerT_ TokenizerT;
struct TokenizerT_ {
	char * _str; /* Points to allocated string copy OR better to use array ? */
	size_t _strLen; /* size */
	int _processedLen ; /* length of string processed*/
	STATE _state ;  /*current state of tokenizer*/
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
int processComment(char *p, TokenizerT *tk);
void printBadToken(char * token);
int isC_OP(char *p);
char * findC_Op(char * token) ;
int checkForSpecialC_OP(char * p);
int isSpecialChar(char *p);

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
	res->_strLen = strlen(ts) +1;
	
	// Allocate string space 	
	res->_str = (char *)malloc(sizeof(char)*res->_strLen);
	if(res->_str == NULL)
		return NULL;
	strcpy(res->_str , ts);
	//PreprocessesString(res->_str);
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

		//if *p=='/' && it is the start of a new token then check for a comment
		if(*p == '/' && tk->_state == START) {
			p = p + processComment(p,tk);
			tokenBeg = tk->_processedLen;
		}
		//check for special c ops like Conditional Expression, >>= <<=, and sizeof()
		if(tk->_state == START && isSpecialChar(p)) {
			int specialCharReturn = checkForSpecialC_OP(p);
			if(specialCharReturn !=0) {
				p = p + specialCharReturn;
				tk->_processedLen += specialCharReturn;
				tk->_state = C_OP;
				break;
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
			 //causing ! error
			__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);


			++tk->_processedLen; // we disregard this character during the next try

			if(!isSpace(token) && tokenLen != 0){
				return token;		
			}
		} 

		prevState = tk->_state;

		tk->_state = stateAndCharTest(p,tk);	
		if(tk->_state == C_OP){ 
			//if a C_OP is found, look if it can have a valid c_op after. IE + can have = after. [ cannot have anything after. 
			//if it can't have anything after break the loop. So if it sees '(' it breaks. If it sees < it checks next value in input string
			int c_opLength = isC_OP(p);
			if( c_opLength== 1){
				//c_op is length of 1 so  increment by 1 then break out to return token
				++tk->_processedLen;
				++p;
				break;
			}else if(c_opLength ==2){
				//c_op is length of 2 so  increment by 2 then break out to return token
				tk->_processedLen += 2;
				p += 2;
				break;
			}else {
				//*p is a punct but not a c_op. Shouldn't go here but just in case
				tk->_state = BADTOKEN;
				p++;
				tk->_processedLen++;
				break;
			}
		}

		if(tk->_state == INDEF || tk->_state == BADTOKEN)
			break;
		
		++tk->_processedLen;
		++p;
	}
	int tokenLen =	tk->_processedLen - tokenBeg ;  
	char * token = (char*)malloc(sizeof(char)* (tokenLen + 1) ) ;
	if (token == 0)
		return NULL;

	__strncpy(token , ( tk->_str  + tokenBeg) , tokenLen);	
	//handles if there is no space and tokenLen != 0 IE 132ASF is a digit and a word
	if(!isSpace(token) && tokenLen != 0 )
	{

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
 *checks if p++ == / or *
 *if p++ == / then that means it is a comment like //
 * // comment stops when *p is a whitespace character
 * / * type of comment only stops when * / is found or end of the string
 *then it is a comment then loop through everything
 *returns number of ints to increment p
*/
int processComment(char *p, TokenizerT *tk) {
	p++;
	tk->_processedLen++;
	int returnVal =1 ;
	if(*p=='/'){
		p++;
		tk->_processedLen++;
		returnVal++;
		while(*p != '\0' && !isspace(*p)) {
			returnVal++;
			p++;
			tk->_processedLen++;
		}
	}else if(*p=='*'){
		returnVal++;
		p++;
		tk->_processedLen++;
		while(*p != '\0') {
			if(*p == '*'){
				returnVal++;
				p++;
				tk->_processedLen++;
				if(*p =='/'){
					returnVal++;
					p++;
					tk->_processedLen++;
					break;
				}else{
					returnVal--;
					p--;
					tk->_processedLen--;
				} 
			}
			returnVal++;
			p++;
			tk->_processedLen++;
		}
	} else{
		returnVal=0;
		p--;
		tk->_processedLen--;
	}
	return returnVal;
}
/*Test to see what character is what */
STATE stateAndCharTest(char *p , TokenizerT * tk)
{
	STATE st ;
	switch(tk->_state)
	{
		case ZERO:
		    if(isX(p))
				st = POTENTIALHEX;
			else if(isOctal(p))
				st = OCTAL;
			else 
				st = INDEF;
			break; 
		case OCTAL:
			if(isOctal(p))
				st = OCTAL;
			else
				st = INDEF;
			break;
		case POTENTIALHEX:
			if(isHex(p))
				st=HEX;
			else
				st = BADTOKEN;
		case HEX:
			if(isHex(p))
				st = HEX;				
			else
				st = INDEF;
			break;

		case DOT:
			if(isDigit(p))
				st = FLOAT;
			else
				st = INDEF;
			break;

		case DIGIT:
			if(isDigit(p))	
				st = DIGIT; 
			else if(isDot(p))
				st = DOT;
			else
				st = INDEF;
			break;

		case EXP:
			if(isPlusMinus(p))
				st = PLUS_OR_MIN;
			else if(isDigit(p))
				st = FLOATE;
			else 
				st = BADTOKEN;
			break;
		case FLOATE:
			if(isDigit(p))
				st = FLOATE;
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
				st = FLOATE;
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
			else if(ispunct(*p))
				st = C_OP;
			else
				st = BADTOKEN;
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

/*Sees if C_OP can have another valid C_OP after it. + can have = after it, but [ cannot have anything after it.
 *only give c_op enough spaced for three, first two chars are potentially a c_op. Last char is null terminator
 *if it should be a one char c_op returns 1
 *if it should be a two char c_op returns 2
 *if not in c_op type array returns 0 but a punctuation-- should never return 0;
 *i is looping variable
 *this handles c ops of length <=2 
 *larger c ops are handled by checkForSpecialC_OP
*/
int isC_OP(char *p) {
	char * c_op = (char *)malloc(sizeof(char)*3);
	if(c_op != 0){
		__strncpy(c_op, p, 2);
		c_op[2] = '\0';
		if(findC_Op(c_op) == 0){
			//setup c_op to be a char * that only has the value *p
			c_op = (char *)realloc(c_op,2);
			if(c_op != 0) {

				c_op[0]= *p;
				c_op[1]='\0';

				if(findC_Op(c_op) == 0){
					//*p is a valid punct char but not a c_op
					free(c_op);
					return 0;
				
				} else {
					//*p is a valid c_op
					free(c_op);
					return 1;
				}

			} else {
				printf("Realloc failed while trying to create new C_OP token");
				return 0;
			}
		} else {
			//this means that first the two chars in c_op are a valid c_op
			free(c_op);
			return 2;
		}
    } 
    printf("Malloc failed while trying to create new C_OP token");
    return 0;
}
/*checks for special c comments >>= sizeof() ?true :false 
 *if >>=  or <<=then return 3
 *if sizeof() return 8
 *if ?true :false return 12
 *what it returns is how far the char p is incremented 
*/
int checkForSpecialC_OP(char * p){
	char * SIZE ="sizeof()";
	char * Conditional = "?true :false";
	size_t sizeOfP = strlen(p) +1;
	if(*p=='<'){
		++p;
		if(*p == '<') {
			++p;
			if(*p=='=')
				return 3;
		}
	return 0;
	} else if(*p=='>'){
		++p;
		if(*p == '>') {
			++p;
			if(*p=='=')
				return 3;
		}
	return 0;
	} 
	//if not <<= or >>= then sizeOfP has to be at least 9 to be sizeOf and at least 13 to be Condititonal
	//copies 8 chars from p (length of sizeof()) then uses strcmp to see if they are the same string, if not and sizeOfP >=13 then does same to Conditional
	else if (sizeOfP >=9) {
		char * selectCopyP = (char *)malloc(sizeof(char)*9);
		if(selectCopyP == 0)
			return 0;
		__strncpy(selectCopyP,p,8);
		selectCopyP[8]='\0';
		if (0 == strcmp(selectCopyP,SIZE)){
			free(selectCopyP);
			return 8;
		} else if(sizeOfP >= 13){
			selectCopyP = (char *)realloc(selectCopyP,sizeof(char)*13);
			if(selectCopyP == 0)
				return 0;
			__strncpy(selectCopyP,p,12);
			selectCopyP[12]='\0';
			if (0 == strcmp(selectCopyP,Conditional)) {
				free(selectCopyP);
				return 12;
			}
		}
		free(selectCopyP);
	}
	return 0;

}
/*Takes in a char * token
 *the token is matched to an entry in tokenArray
 *returns the tokenType if found in the array
 *Worst case: O(n) where n is the amount of tokens in tokenArray
 *if not found and it is a punctuation then returns null
*/

char *  findC_Op(char * token) {
	//printf("token: %s\n",token);
	char **tokenArray = (char *[]){"[","]","{","}",".","&","*","-","/","%","+","=",";","<",">","!","~",",","+=","-=","*=",">>","<<","<=",">=","==","!=","&&","||","/=","&=","|=","%=",">>=","<<=","^","^=",":","(",")","sizeof()","?true :false","|","->"};
	char ** tokenType = (char *[]){"leftbrace", "rightbrace","leftcurlybrace","rightcurlybrace","dot","address","multiply","minus","divide","modulus","plus","equals","semicolon","lessthan","greaterthan","negate","1's comp","comma","plusequals","minusequals","multiplyequals","shift right","shift left","less or equal","greater or equal","equals","not equals","Logical and","Logical or","divide equals","and equals","or equals","modulo equals","shiftrightequals","shiftleftequals","bitwise exclusive or","exclusive or equals","colon","left parenthesis","right parenthesis","cast","Conditional Expression","bitwise or","structure pointer"};
	int tokenArraySize = 44;
	int i = 0;
	for(i = 0; i < tokenArraySize ; i++) {
		if(0 ==strcmp(token,tokenArray[i])){
			return tokenType[i];
		}
	}
	return 0;
}


/*Bad tokens need to have their hex values printed 
 *Loops through token and prints out the specfic unsigned int
 *Convert to unsigned char then print out 
 *uses multiple printf statements and %ux
*/
void printBadToken(char * token) {
	size_t token_len = strlen(token);
	int i = 0;
	printf("Bad Token: 0x");
	for(i =0; i < token_len; i++)
		printf("%x",(unsigned char)token[i]);
	printf("\n");

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
	else if(tk->_state == FLOAT ||tk->_state == FLOATE)
		printf("float \"%s\"\n",token);
	else if(tk->_state == HEX)
		printf("hex \"%s\"\n",token);
	else if(tk->_state == ZERO)
		printf("zero \"%s\"\n",token);
	else if (tk->_state == WORD)
		printf("word \"%s\"\n",token);
	else if(tk->_state ==C_OP) {
		printf("%s \"%s\"\a \n",findC_Op(token),token);
	}
	else 
		printBadToken(token);

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
int isSpecialChar(char *a){
	return(*a =='s' || *a == '<' || *a == '>' || *a == '?');
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
	return isalpha(*a); 
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
	return (*a=='e');
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

	while(1)
	{
		char * res = TKGetNextToken(tk);	
		if(res == 0)
			break ; 
		stateTokenPrint(res,tk);
		if(res != 0)
			free(res);
	}	

	TKDestroy(tk);
  return 0;
}

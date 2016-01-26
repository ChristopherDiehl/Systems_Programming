/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Defines*/
#define GENERALERROR "An error occured while processing your request. \nPlease try again later. \n"
#define ARGMISMATCHERROR "Invalid arguments.\n"
#define NULLARGS "Null Value\n"
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

typedef struct _Token {
	char *token;
	char *type;
}Token;

typedef struct TokenizerT_ {
	char *st;
	Token *tokens;
	int numOfTokens;
} TokenizerT;

TokenizerT *TKCreate(char * ts);
Token *newToken(char *token, char *type);
void deleteToken(Token *delToken);
void TKDestroy( TokenizerT * tk);
char *TKGetNextToken(TokenizerT * tk);
void PreProcessString(char *ts, size_t ts_length);
char **HuntForTokens(char *ts, size_t ts_length);
int ArrayIsFull(int numOfEntries, int sizeOfArray);
int ExpandTokenArray(Token *tokens, size_t stringLength, int sizeOfArray);
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
 *
 */

TokenizerT *TKCreate( char * ts ) {
  TokenizerT *tokenizer;
  size_t ts_length = strlen(ts) +1;
  if(ts_length == 1)
  	return NULL;
  char *cp_ts = malloc(ts_length);
  strcpy (cp_ts, ts);
  PreProcessString(cp_ts,ts_length);
  HuntForTokens(cp_ts,ts_length);

  return NULL;
} 
/*PreProccessString takes in the copy of the token stream.
 * 
 *takes in a copy of the input string and the length of the input string
 *PreProcessString gets rid of all the deliminators and replaces them with '\0'
 *This is done so the only delim that needs checked for is '\0'
 *
 *If functions succeeds it return a processed string
 *Else it prints an error message and return null;
*/
void PreProcessString(char *ts, size_t ts_length) {
	int i = 0;
	for(i = 0; i < ts_length; i++){
		switch(ts[i]) {
			case ' ':
				ts[i] = '\0';
			case '\t':
				ts[i] = '\0';
			case '\v':
				ts[i] = '\0';
			case '\f':
				ts[i] = '\0';
			case '\n':
				ts[i] = '\0';
			case '\r':
				ts[i] = '\0';
		}
	}

}
/*
 *HuntForTokens takes in the copied string that is already processed, the length of the copied string, and a pointer to the number
 *of tokens. I did this so that I can edit the value of numOfTokens while returning the tokens.
 *Looks through the copied string for Tokens
 *Returns an array of string which are the tokens
 *If user sends in an empty string return NULL to stop method
 *
 *char typeChar is used to keep track of previous char in case character is a 0
 *meant to handle octal or hex case
 *
 *i is the looping variable in this function
 *start keeps track of what i was when it entered an if statement
 *ie ts="HELLO", goes into isalpha and start = 0. Start is used for malloc and stnrcpy
 *
 *The array of tokens is initiallized to size 2
 *The size expands by two.
 *The current size of the token array is held in tokenArraySize
 *when numOfTokens > tokenArraySize realloc expands the token array size
*/
 char **HuntForTokens(char *ts, size_t ts_length) {
 int i = 0;
 int start = 0;
 int numOfTokens = 0;
 size_t tokenArraySize =2;
 Token **tokens = malloc(sizeof(Token)*tokenArraySize);
 for(i=0; i <ts_length; i++ ) {
 	if(isalpha(ts[i])) {
 	 	start = i;
 		while(isalpha(ts[i]) && ts[i] != '\0') {-
 			i++;
 		} 
	 	i--;

 		char *token = malloc(sizeof(char)*(i-start)+1);
 		strncpy(token, &ts[start], i-start+1);
	 	tokens[numOfTokens] = newToken(token,"Word");
	 	numOfTokens ++;
	 	printf("TOKEN: %s \nTYPE: %s \n",tokens[numOfTokens-1]->token,tokens[numOfTokens-1]->type);
 	} else if(isdigit(ts[i])) {
 		start = i;
 		if(ts[i]==0){
 			char typeC = ts[i];
 			i++;
 			if(ts[i]=='x'){
 				if(isxdigit(ts[i+1])){
 					i = i +2;
 					while(isxdigit(ts[i])){
 						i++;
 					}
 					i--;
 					char *token = malloc(sizeof(char)*(i-start)+1);
			 		strncpy(token, &ts[start], i-start+1);
				 	tokens[numOfTokens] = newToken(token,"Hexadecimal");
				 	numOfTokens ++;
 				} else {
 					//handles cases like 0xZ
 					char *token = 0;
 					tokens[numOfTokens] = newToken(token,"Decimal");
 					numOfTokens++;
 					if(1==ArrayIsFull(numOfTokens,tokenArraySize))
	 					tokenArraySize = ExpandTokenArray(*tokens,ts_length,tokenArraySize);
 					tokens[numOfTokens] = newToken("x","Word");
	 				numOfTokens++;
 					
 				}
 			}
 		} else{
 			i++;
 			while(isdigit(ts[i])){
 				i++;
 			}
 			i--;
			char *token = malloc(sizeof(char)*(i-start)+1);
	 		strncpy(token, &ts[start], i-start+1);
		 	tokens[numOfTokens] = newToken(token,"Decimal");
		 	numOfTokens ++;
		 	printf("TOKEN: %s \nTYPE: %s \n",tokens[numOfTokens-1]->token,tokens[numOfTokens-1]->type);
 		}
 	}
	if(1 ==ArrayIsFull(numOfTokens,tokenArraySize)){
		tokenArraySize = ExpandTokenArray(*tokens,ts_length,tokenArraySize);
	}
 }
 
 return NULL;

}
/*ExpandTokenArray
 *expands tokenArrayBy twice its original size up until the size of the input string
 *there will be at most n tokens in an n length input string
 *accepts pointer to change value
 *arguments: Array of tokens, int which is input string length, int current size of token array
 *returns an int which is the new size of the array
*/
int ExpandTokenArray(Token *tokens, size_t stringLength, int sizeOfArray){
	sizeOfArray = sizeOfArray*2;
		if(sizeOfArray > stringLength)
			sizeOfArray = stringLength;
	tokens = realloc(tokens,sizeOfArray);
	return sizeOfArray;
}

/*ArrayIsFull(int numOfEntries, int sizeOfArray)
 *if array is full returns 1, else returns 0
 *used to see if numOfTokens == tokenArraySize
*/
int ArrayIsFull(int numOfEntries, int sizeOfArray){
	if(numOfEntries == sizeOfArray){
		return 1;
	}
	return 0;
}
/*Function to create a new Token
 *
 *Takes in a char * for the token and a char * for the type
 *
 *return a new Token struct
*/
 Token *newToken(char *token, char *type) {
 	Token *newToken= malloc (sizeof(Token));
 	newToken->token = malloc(sizeof(token));
 	newToken->type = malloc(sizeof(type));
 
 	newToken->token = token;
 	newToken->type = type;
 	return newToken;
 }

/*Delete Token takes in a *Token delToken and destroys it using free
 *
 *if succesfull returns 0, else returns 1;
*/
 void deleteToken(Token *delToken){
 	if(delToken != NULL) {
 		free(delToken->token);
 		free(delToken->type);
 		free(delToken);
 		
 	} else printf(NULLARGS);
 	
 }

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 *
 * Loops through and frees up the memory taken up by tokens
 *i is the looping variable
 */

void TKDestroy( TokenizerT * tk ) {
	if(tk != NULL){
		int i = 0;
	
	} else {
		printf(NULLARGS);
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

  return NULL;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	TokenizerT *tokenizer;
	if (argc != 2) {
		printf("%s",ARGMISMATCHERROR);
		return -1;
	}
	TKCreate(argv[1]);
  return 0;
}
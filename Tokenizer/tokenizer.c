/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
};
typedef struct TokenizerT_ TokenizerT;

TokenizerT *TKCreate(char * ts);
Token *newToken(char *token, char *type);
void deleteToken(Token *delToken);
void TKDestroy( TokenizerT * tk);
char *TKGetNextToken(TokenizerT * tk);
void PreProcessString(char *ts, size_t ts_length);
char **HuntForTokens(char *ts, size_t ts_length, int *numOfTokens);
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
  int *numberOfTokens = 0;
  if(ts_length == 1)
  	return NULL;
  char *cp_ts = malloc(ts_length);
  strcpy (cp_ts, ts);
  PreProcessString(cp_ts,ts_length);
  HuntForTokens(cp_ts,ts_length,numberOfTokens);

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
	int *numberOfTokens =0;
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
 *int type key
 *0 = no type. If new token
 *1 = word
 *2 = decimal
 *3 = float
 *4 = octal
 *5 = hex
 *
 *i is the looping variable in this function
*/
 char **HuntForTokens(char *ts, size_t ts_length, int *numOfTokens) {
 int type = 0;
 int i =0;
 Token *tokens = malloc(sizeof(Token));
 for(i=0; i <ts_length; i++ ) {
 	if(isalpha(ts[i])) {
 		while(isalpha(ts[i])) {
 			printf("%c",ts[i]);
 			i++;
 		}


 	}
 }
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
 }

/*Delete Token takes in a *Token delToken and destroys it using free
 *
 *
*/
 void deleteToken(Token *delToken){
 	if(delToken != NULL) {
 		free(delToken->token);
 		free(delToken->type);
 		free(delToken);
 	}
 }

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
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
	char *argMismatch = "Invalid arguments. \n";
	char *generalError = "An error occured while processing your request. \nPlease try again later. \n";
	if (argc != 2) {
		printf("%s",argMismatch);
		return -1;
	}
	TKCreate(argv[1]);
  return 0;
}
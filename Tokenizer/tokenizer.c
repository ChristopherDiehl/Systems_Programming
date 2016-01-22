/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void TKDestroy( TokenizerT * tk);
char *TKGetNextToken(TokenizerT * tk);
char *PreProcessString(char * ts);

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
 *If user sends in an empty string return NULL to stop method
 *
 */

TokenizerT *TKCreate( char * ts ) {
  TokenizerT *tokenizer;
  size_t ts_length = strlen(ts) +1;
  if(ts_length == 1)
  	return NULL;
  char *cp_ts = malloc(ts_length);
  strcpy (cp_ts, ts);
  return NULL;
} 
/*PreProccessString takes in the copy of the token stream.
 * 
 *PreProcessString gets rid of all the deliminators and replaces them with '\0'
 *This is done so the only delim that needs checked for is '\0'
 *
 *If functions succeeds it return a processed string
 *Else it prints an error message and return null;
*/
char *PreProcessString(char *ts) {
	size_t ts_length = strlen(ts);
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
	return ts;
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
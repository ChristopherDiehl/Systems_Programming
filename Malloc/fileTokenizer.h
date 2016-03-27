#ifndef FILE_TOKENIZER_H
#define FILE_TOKENIZER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*Tokenizer*/




typedef enum { START ,DIGIT  WORD,INDEF,BADTOKEN } STATE ;

typedef struct TokenizerT_ TokenizerT;
struct TokenizerT_ {
	char * _str; // Points to allocated string copy OR better to use array ? 
	size_t _strLen; // size
	int _processedLen ; // length of string processed
	STATE _state ;  //current state of tokenizer
};


TokenizerT * Tokenize(char * file);
char * GetToken(TokenizerT * tk);
void tkDestroy( TokenizerT * tk);

int isDigit(char *a);
int isWord(char *a);
int isSpace(char *a);
STATE stateAndCharTest(char *p , TokenizerT * tk);
int __strncpy(char * dest, char * src , int len);


#endif

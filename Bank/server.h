#ifndef SERVER_H
#define SERVER_H


#define MAX 100
#define TRUE 1
#define FALSE 0
#define PORT 6512
#define BUFFER_SIZE 256
#define CLIENT_THREADS 20
#define MAX_CLIENTS 20

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <ctype.h> // to obtain the token


/*
typedef struct Account_
{
	char * username;
	float balance;
	int active;

}Account;

typedef struct Bank_
{
	Account * accounts;

}Bank;

*/  // Using san'a implementation of bank

/*FUNCTIONS*/
void * connectionHandler( void * socket);
void error(char * msg);
void * sessionAcceptor( void * socket);
void * printBankStatus (void * socket);
void sigHandler(int dummy);




/*TOKEN EXTRACTION */
int __strncpy(char * dest, char * src , int len);


#endif

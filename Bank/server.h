#ifndef SERVER_H
#define SERVER_H


#define MAX 100
#define TRUE 1
#define FALSE 0
#define PORT 6512
typedef struct account_
{
	char * name;
	float balance;
	int active;


} account;

typedef struct bank_
{
	account * accounts;

};

/*FUNCTIONS*/
void * connectionHandler( void * socket);
void error(char *msg);

#endif

// Account struct to handle the account :
// The accounts are unique to each thread hence we do not need a mutex within the thread
// But the bank is used by 20 threads hence we need a mutex within the bank struct
//
//
// Bank object will be a global variable in the server , so that all the threads in the server can add accounts to it 
//
//  Good explantion of using pthreads 
// http://www.linuxjournal.com/article/3138
//
//

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define N_CLIENT 20
typedef struct account_
{
	char *_name ;  // 
	float _balance ; 
	int _inUse ; // 1 or 0 

} account ; 

account * createAccount( char * name , float bal);


/*
 * holds 20 accounts in an array 
 */
typedef struct bank_
{
	account	 * accountArray[N_CLIENT] ;  
	int _accountsUsed ; 

	// mutex lock  exclusive of the bank : when this locked a thread cannot create an acoount or changing within the account 
	pthread_mutex_t acnt_mutex ;

} bank; 


bank * createBank();
int startAccount(bank * bk , char * name);
int openAccount( bank * bk , char * name , float bal);
int changeBalAccount(bank * bk , int accIdx , float posNegVal);
int creditAccount(bank * bk , int accIdx , float credit ) ;
int debitAccount(bank * bk , int accIdx , float debit ) ;
float  balanceAccount(bank * bk , int accIdx);
void finishAccount(bank * bk , int accIdx);



/*
 * RULES FOR BANK :: 
 * 	While prinitng the account names : No new account can be created :: 
 * 
 * 	open accountName : sets up an account but not in use 
 * 	only clients not in customer session can open accounts 
 *
 */

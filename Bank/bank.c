#include "bank.h"


/*
 * create the account placed in the bank 
 *
 */
account * createAccount( char * name , float bal)
{
	account * res =  ( account*) malloc(sizeof(account)) ; 
	res->_name =  (char *)malloc(100);         	 // malloc 100 bytes;
	// copy name to _name ; 
	res->_name = strcpy(res->_name , name );
	res->_balance = bal ; 

	/* 
	 * account becomes is use during the start accountname command 
	 * not on creation 
	 */
	res->_inUse = 0; 
	return res; 
}



bank * createBank()
{
	bank * res = (bank *)malloc(sizeof(bank)); 
	
	// the accountArray is initialized when an account is added ; 	
	res->_accountsUsed = 0; 
	res->acnt_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; 		
	
	return res ; 
}


void destroyBank(bank * bk)
{
	int i = 0;
	for(i = 0; i < bk->_accountsUsed; i++)
	{
		free(bk->accountArray[i]->_name);
		free(bk->accountArray[i]);

	}
	free(bk);
}
/*
 * create the account : 
 * 	lock the mutex and add the account 
 * 
 * a consumer cannot add an account 
 * take care of this 
 *
 *
 * return 
 * 	-1 ( failure due to # acc > 20 
 * 	-2 ( failure due to non unqiue account # )
 * 	[0-19] success 
 *
 *
 * client has to know which account was added ? 
 *
 * We also have to ensure that accounts with duplicate ames are not added 
 *
 * TO DO :: 
 * if a clinet has worked with an account then if shoudl not add a new account 
 *
 * How to ensure this ? With in the client , because add account fails if it already exists 
 */


int openAccount( bank * bk , char * name , float bal )
{

	// ensure that only one thread uses this at a time 
	// Should this be used only when #acc < 20 ? 
	// this seems cleaner ? 
	//
	pthread_mutex_lock(&bk->acnt_mutex);

	// The comparison also has to be locked to ensure that new account is added 
	// during the comprison for unique names ! 

	int res = 0 ; 

	// Make sure that the account that is being added is unique 	

	int i = 0; 
	for( i =0 ; i < bk->_accountsUsed ; ++i)
	{
		if( strcmp(name , bk->accountArray[i]->_name) == 0 )
		{
			if(DEV) printf(" non unique account name !");
			res = -2 ; 		

		}

	}
	
	if( res == 0)
	{
		if (bk->_accountsUsed < 20 && ( res == 0 ) )
		{
			// add the account 
			bk->accountArray[bk->_accountsUsed] = createAccount(name , bal);
			bk->_accountsUsed++ ; 
			res = 1; 
		//	return 1; 	
		}
		else
		{
			res = -1 ; 
			//return 0 ; 
		}
	}	
	// Warning :: Unsure 
	pthread_mutex_unlock(&bk->acnt_mutex) ; 
	return res; 
}


/*
 *
 * return 
 * 1 : succcess
 * -1  : failure cannot find the requested account 
 * !!!! IMP !!!
 * -2 : cannot put the account in inUse as it is already in inUse by another client 
 * The client should not let the user start a session if it receives this signal 
 * as on starting a session on receiving this, different clinets will be concurrently using the 
 * same account and making changes to it .. .. 
 *
 * Should there be locking ? 
 * 	No : each client only adds a single account 
 * 	so within the account the 
 *
 *
 * Warning :: 
 * 	Supppose a client create an account, can another clinet use that account and
 * 	make changes to it ? 
 * 	If so we will have to make sure that there is locking for each one of the 20 accounts:
 * 	if one client is making changes to an account , another clinet could also make 
 * 	changes.. 
 *	
 *	is the mapping between clinet and account 1 to 1 ? 
 */
int startAccount(bank * bk , char * name)
{
	if(DEV) printf("STARTING ACCOUNT : %s \n" , name);
	int i = 0 ; 
	for( i =0 ; i < bk->_accountsUsed ; ++i)
	{
		
		if( strcmp(name , bk->accountArray[i]->_name) == 0 )
		{
			// Found the account
			// Make the account in use :  
			// ensure that concurrent sessions do not exist 
			if(  bk->accountArray[i]->_inUse == 0 ) 
			{
				bk->accountArray[i]->_inUse = 1; 
				return i; 
			}
			else
			{
				if(DEV) printf("[-] Account Already in session cannot open again\n");
				return -2 ; 
			}
		}
	}
	if(DEV) printf("[-] Such an account has not been opened yet \n");
	return -1 ; 
}


/*
 *
 * addIdx : 
 * 	store the return value of start or open and 
 * 	use it to obtain an index into the array of accounts 
 * 	and use this index to access a particular account 
 *	
 *	This is a better system than using the names as the commands 
 *	credit 
 *	debit 
 *	balance
 *	finish
 *	exit
 *
 *	does not have an account name to index the array : 
 *
 *	The client or the client handler in the server should remember this number 
 *
 *
 * return codes :
 * 1 change balance
 * -1 unable to change balance due to account not start 
 * 
 */
int changeBalAccount(bank * bk , int accIdx , float posNegVal)
{
		// the balance can be changed only if the account has been started ? 
		// VERIFY THIS ! 
	
	if(DEV)	printf("change balance id : %d , val %f " , accIdx , posNegVal);
	if (bk->accountArray[accIdx]->_inUse ==1 )
	{
		bk->accountArray[accIdx]->_balance += posNegVal ; 
		return 1; 
	}
	else
	{
		return -1; 
	}

}

/*
 * credit means add value ? 
 */
int creditAccount(bank * bk , int accIdx , float credit ) 
{
	return changeBalAccount(bk , accIdx , credit); 
}

/*
 *  debit means sub value ? 
 *
 *  return 
 *  -1 : failure : Insufficeint Funds 
 *  1 : Sufficient Funds and val removed from bank  
 */
int debitAccount(bank * bk , int accIdx , float debit ) 
{
	// No checking happens here :: We have to ensure that the account is in use ? 
	// Or should we ?? 
	if (bk->accountArray[accIdx]->_inUse ==1 )
	{
		float temp = bk->accountArray[accIdx]->_balance;
		if(temp + debit < 0)
		{
			return -1;
		}
		else{
			bk->accountArray[accIdx]->_balance += debit;
			return 1; 

		}
	}
	else
	{
		return -1; 
	}

	
}

/*
 * should the account be in use for the balance to be extracted ?
 * yes ? or no ? 
 * should start have been called before obtaining the balance ? 
 * 
 * Instruction : 
 * 	balance can be obtained within a session 
 * WARNING VERIFY 
 */
float  balanceAccount(bank * bk , int accIdx)
{
	if (bk->accountArray[accIdx]->_inUse ==1 ) // not required the server will handle this
	{
		return 	bk->accountArray[accIdx]->_balance ; 
	}
	else
	{
		return -1;  // This is the wrong way to do it , how to pass error to user erno ? 
	}

}

/*
 * what is an accout is not inuse ? 
 *
 * Will the user stick to these contracts or do we have to force it ? 
 *
 */
void finishAccount(bank * bk , int accIdx)
{

	bk->accountArray[accIdx]->_inUse = 0 ;
	/*
	if (bk->accountArray[accIdx]->_inUse ==1 )
	{

		bk->accountArray[accIdx]->_inUse = 0 ;
	}
	else
	{
		return -1;  // This is the wrong way to do it , how to pass error to user erno ? 
	}
	*/
}



/*
 * how to ensure that the different clients dont use the same account together ?  
 *	 we know that changes can be made to an account only when it has been put inUse 
 *
 *
 * If an account is in use then it should not be put in use by another clinet[
 *
 *
 *
 */


int getAccountNum(bank * bk , char * accName)
{
	int i = 0 ; 
	for( i =0 ; i < bk->_accountsUsed ; ++i)
	{
		
		if( strcmp(accName , bk->accountArray[i]->_name) == 0 )
		{
			// Found the account
			// Make the account in use :  
			// ensure that concurrent sessions do not exist 
			if(  bk->accountArray[i]->_inUse == 0 ) 
			{
				bk->accountArray[i]->_inUse = 1; 
				return i; 
			}
		}

	}


	return -1;

}












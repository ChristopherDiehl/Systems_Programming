/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

/*STATIC VARIABLES*/
pthread_t c_threads[MAX_CLIENTS];		//threads solely for handling client connections
Bank * bank;
Account * account;




int main(int argc, char *argv[])
{
	//int sockfd, newsockfd, portno, clilen;
	//struct sockaddr_in serv_addr;
	//struct sockaddr_in cli_addr;

	int sockfd = -1;														// file descriptor for our server socket
	int newsockfd = -1;												// file descriptor for a client socket
	int portno = -1;														// server port to connect to
	int clilen = -1;															// utility variable - size of clientAddressInfo below
	int n = -1;																// utility variable - for monitoring reading/writing from/to the socket
	char buffer[BUFFER_SIZE];													// char array to store data going to and coming from the socket
	int err = -1;
	struct sockaddr_in serverAddressInfo;				// Super-special secret C struct that holds address info for building our server socket
	struct sockaddr_in clientAddressInfo;					// Super-special secret C struct that holds address info about our client socket
	bank = malloc(sizeof(Bank));
	bank->accounts = malloc(sizeof(Account) * MAX_CLIENTS);


	// try to build a socket .. if it doesn't work, complain and exit
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
   if (err != 0) 
   {
   	error("[-] ERROR creating socket");
   }
	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(PORT);

	// set a flag to indicate the type of network address we'll be using  
	serverAddressInfo.sin_family = AF_INET;

	// set a flag to indicate the type of network address we'll be willing to accept connections from
	serverAddressInfo.sin_addr.s_addr = INADDR_ANY;



	/** We have an address struct and a socket .. time to build up the server socket **/

	// bind the server socket to a specific local port, so the client has a target to connect to      
	if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		error("[-] ERROR on binding");
	}

	// set up the server socket to listen for client connections
	listen(sockfd,5);

	// determine the size of a clientAddressInfo struct
	clilen = sizeof(clientAddressInfo);

	//multithread this
	while(newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen))
	{

		if (newsockfd < 0) 
		{
			error("[-] ERROR on accept");
		}
		break;
	}



	/** If we're here, a client tried to connect **/

	// if the connection blew up for some reason, complain and exit

	// zero out the char buffer to receive a client message
	bzero(buffer,BUFFER_SIZE);

	// try to read from the client socket
	n = read(newsockfd,buffer,BUFFER_SIZE -1);

	// if the read from the client blew up, complain and exit
	if (n < 0)
	{
		error("[-]ERROR reading from socket");
	}

	printf("Here is the message: %s\n",buffer);

	// try to write to the client socket
	n = write(newsockfd,"I got your message",BUFFER_SIZE);
	// if the write to the client below up, complain and exit

	if (n < 0)
	{
		error("ERROR writing to socket");
	}
	
	return 0; 
}

/*This handles client connection*/
void * connectionHandler( void * socket)
{
	return 0;
}

void * printBankStatus (void * socket)
{
	int i = 0;

	if(bank == NULL)
	{
		printf("[-]Something went horribly wrong. Bank not initialized");
		return 0;
	}

	for(i = 0; i < MAX_CLIENTS; i++)
	{

		if(bank->accounts[i] == NULL)
		{

			continue;

		} else if(bank->accounts[i]->active == TRUE)
		{

			printf("%s, %f,IN SERVICE\n",bank->accounts[i]->username,bank->accounts[i]->balance);

		} else if(bank->accounts[i]->active == FALSE)
		{

			printf("%s, %f\n",bank->accounts[i]->username,bank->accounts[i]->balance);

		}
	}
	sleep(20);
	return 0;
}

void error(char *msg)
{
	perror(msg);
	exit(1);
}
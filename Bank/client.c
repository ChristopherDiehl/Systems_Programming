#include "client.h"

/*Static Variables*/
pthread_t threads[2];
int end = 0;
char command_buffer[256];													// char array to store data  going to the server
char response_buffer[256];														// char array to store data  coming from the server


int main(int argc, char *argv[])
{
	
	// Declare initial vars
   int sockfd = -1;																// file descriptor for our socket
   int portno = -1;																// server port to connect to
   struct sockaddr_in serverAddressInfo;						// Super-special secret C struct that holds address info for building our socket
   struct hostent *serverIPAddress;									// Super-special secret C struct that holds info about a machine's address
   int iSetOption = 1;
   int err = -1;
	
	
	// If the user didn't enter enough arguments, complain and exit
   if (argc < 3)
   {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	
	
	/** If the user gave enough arguments, try to use them to get a port number and address **/
	// convert the text representation of the port number given by the user to an int
	portno = atoi(argv[2]);
	
	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
   
   serverIPAddress = gethostbyname(argv[1]);
   if (serverIPAddress == NULL)
   {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
   }
				
	// try to build a socket .. if it doesn't work, complain and exit
   sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sockfd < 0) 
   {
      error("ERROR creating socket");
   }

	
   err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
   if (sockfd < 0) 
   {
      error("ERROR creating socket");
   }

	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!
   bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
   serverAddressInfo.sin_family = AF_INET;
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
   serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
   bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);



   
   while(connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) != 0) 
   {
   	printf("[-] ERROR connecting");
   	sleep(3);
   }
		
	/** If we're here, we're connected to the server .. w00t!  Time to multithread**/
   
	err = pthread_create(&(threads[0]), NULL, &writeToServer, (void *) sockfd);

	if (err != 0)
	{
	    error("\n[-]can't create thread :[%s]"); 
	}
	
	err = pthread_create(&(threads[1]), NULL, &readFromServer, (void *) sockfd);

	if (err != 0)
	{
	    error("\n[-]can't create thread :[%s]"); 
	}


	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);	

	return 0;
}

void * writeToServer(void * args)
{
	int sockfd = *((int *) args);
	int n = -1;
	while(end != 1)
	{
		printf("[-] Please enter message");
		// zero out the message buffer
		bzero(command_buffer,256);

		// get a message from the client
		fgets(command_buffer,255,stdin);

		if(strcmp(command_buffer,"exit"))
		{
			end = 1;
		}

		// try to write it out to the server
		n = write(sockfd,command_buffer,strlen(command_buffer));

		if (n < 0)
		{
		   error("ERROR writing to socket");
		}

		sleep(3);
	}
	return 0;
}

void * readFromServer(void * args)
{
	int sockfd = *((int *) args);
	int n = -1;
	while(end != 1)
	{
		bzero(response_buffer,256);

		// read a message from the server into the buffer
		n = read(sockfd,response_buffer,255);

		// if we couldn't read from the server for some reason, complain and exit
		if (n < 0)
		{
		   error("ERROR reading from socket");
		}

		// print out server's message
		printf("%s\n",response_buffer);
	}

	return 0;
}


void error(char *msg)
{
   perror(msg);
   exit(0);
}

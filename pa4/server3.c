#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<pthread.h>
#include    <semaphore.h>
#include    "bank.c"

// Miniature server to exercise getaddrinfo(2).
//static pthread_attr_t	user_attr;
//static pthread_attr_t	kernel_attr;
//static sem_t		actionCycleSemaphore;
static pthread_mutex_t	mutex;
static int		connection_count = 0;

int
claim_port( const char * port )
{
	struct addrinfo		addrinfo;
	struct addrinfo *	result;
	int			sd;
	char			message[256];
	int			on = 1;

	addrinfo.ai_flags = AI_PASSIVE;		// for bind()
	addrinfo.ai_family = AF_INET;		// IPv4 only
	addrinfo.ai_socktype = SOCK_STREAM;	// Want TCP/IP
	addrinfo.ai_protocol = 0;		// Any protocol
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;
	if ( getaddrinfo( 0, port, &addrinfo, &result ) != 0 )		// want port 3000
	{
		fprintf( stderr, "\x1b[1;31mgetaddrinfo( %s ) failed errno is %s.  File %s line %d.\x1b[0m\n", port, strerror( errno ), __FILE__, __LINE__ );
		return -1;
	}
	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )
	{
		write( 1, message, sprintf( message, "socket() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		return -1;
	}
	else if ( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) == -1 )
	{
		write( 1, message, sprintf( message, "setsockopt() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
		freeaddrinfo( result );
		close( sd );
		return -1;
	}
	else if ( bind( sd, result->ai_addr, result->ai_addrlen ) == -1 )
	{
		freeaddrinfo( result );
		close( sd );
		write( 1, message, sprintf( message, "\x1b[2;33mBinding to port %s ...\x1b[0m\n", port ) );
		return -1;
	}
	else
	{
		write( 1, message, sprintf( message,  "\x1b[1;32mSUCCESS : Bind to port %s\x1b[0m\n", port ) );
		freeaddrinfo( result );		
		return sd;			// bind() succeeded;
	}
}

void *
client_session_thread( void * arg )
{
	int			sd;
	char			request[2048];
	//char			response[2048];
	//char			temp;
	//int			i;
	//int			limit, size;
	//float			ignore;
	//long			senderIPaddr;

	sd = *(int *)arg;
	free( arg );					// keeping to memory management covenant
	pthread_detach( pthread_self() );		// Don't join on this thread

	pthread_mutex_lock( &mutex );
	++connection_count;				// multiple clients protected access
	pthread_mutex_unlock( &mutex );
	
	
	while ( read( sd, request, sizeof(request) ))
	{
		
		printf( "server receives input:  %s\n", request );
		char* message;
		
		
		/*0 is to create/access account  */
		if (strcmp(request, "0") == 0) {
			char *compare;
			message = "Enter your name\n";
			write( sd, message, strlen(message) + 1 );
			read( sd, request, sizeof(request));
			compare = createAccount(request);
				if (strcmp(request, compare) == 0) {
					message = "You have sucessfully created an account\n";
					write( sd, message, strlen(message) + 1 );
					continue;
				}
				else{
					write( sd, compare, strlen(compare) + 1 );
					continue;
				}
		}

		/*1 deposit*/
		else if(strcmp(request, "1") == 0){
			message = "Enter name\n";
			write( sd, message, strlen(message) + 1 );
			read( sd, request, sizeof(request));
			char *name;
			name = isaccountthere(request);
			char *getdep;
			char *give;
			float *amount = malloc(sizeof(float));
				
			if (strcmp(name, request) == 0) {
				give = "How much money do you wish to deposit?\n";
			
				write( sd, give, strlen(give) + 1);
				read( sd, request, sizeof(request));
				*amount = atof(request);
				
				//printf("%s%s\n","amount in string" ,request);
				//printf("%s%f\n","the amount here :       ", *amount );
				getdep = deposit(name, *amount);
				write( sd, getdep, strlen(getdep) + 1 );
				continue;
			}
		}

		/*2 if withdraw*/
		else if(strcmp(request, "2") == 0){
			message = "\nEnter name\n";
			write( sd, message, strlen(message) + 1 );
			read( sd, request, sizeof(request));
			char *name;
			name = isaccountthere(request);
			char *getdep;
			char *give;
			float *amount = malloc(sizeof(float));
				
			if (strcmp(name, request) == 0) {
				give = "\nHow much money do you wish to withdraw?\n";
			
				write( sd, give, strlen(give) + 1);
				read( sd, request, sizeof(request));
				*amount = atof(request);
				
				//printf("\n%s%s\n","amount in string: " ,request);
				//printf("%s%f\n","the amount here :       ", *amount );
				getdep = withdraw(name, *amount);
				write( sd, getdep, strlen(getdep) + 1 );
				
				continue;
			}
		}

		/*3 to query it*/
		else if (strcmp(request, "3") == 0) {
			char *compare;
			message = "Enter your name\n";
			write( sd, message, strlen(message) + 1 );
			read( sd, request, sizeof(request));
			compare = query(request);
			write( sd, compare, strlen(compare) + 1 );
			continue;
		}

		/*4 to quit it*/
		else if (strcmp(request, "4") == 0) {
			message = "\nSession has been ended.\n";
			write( sd, message, strlen(message) + 1 );
			close( sd );
			return 0;
		}

		else {
		message = "\ncreate/access-0, deposit-1, withdraw-2, query-3, quit-4\nFor 0: What is the name of the account you wish to access?\nFor 1: How much money do you wish to deposit\nFor 2: How much money do you wish to withdraw\n";
		}


		//message = "How is ti going\n";//works somehow
		
		
		/*
		size = strlen( request );
		limit = strlen( request ) / 2;
		for ( i = 0 ; i < limit ; i++ )
		{
			temp = request[i];
			request[i] = request[size - i - 1];
			request[size - i - 1] = temp;
		}*/

		write( sd, message, strlen(message) + 1 );//has been changed
	}
	pthread_mutex_lock( &mutex );
	--connection_count;				// multiple clients protected access
	pthread_mutex_unlock( &mutex );
	close( sd );
	return 0;
}

int
main( int argc, char ** argv )
{
	int			sd;
	char			message[256];
	pthread_t		tid;
	pthread_attr_t		kernel_attr;
	socklen_t		ic;
	int			fd;
	struct sockaddr_in      senderAddr;
	int *			fdptr;

	if ( pthread_attr_init( &kernel_attr ) != 0 )
	{
		printf( "pthread_attr_init() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}
	else if ( pthread_attr_setscope( &kernel_attr, PTHREAD_SCOPE_SYSTEM ) != 0 )
	{
		printf( "pthread_attr_setscope() failed in file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}
	else if ( (sd = claim_port( "5281" )) == -1 )
	{
		write( 1, message, sprintf( message,  "\x1b[1;31mCould not bind to port %s errno %s\x1b[0m\n", "5281", strerror( errno ) ) );
		return 1;
	}
	else if ( listen( sd, 100 ) == -1 )
	{
		printf( "listen() failed in file %s line %d\n", __FILE__, __LINE__ );
		close( sd );
		return 0;
	}
	else
	{
		ic = sizeof(senderAddr);
		while ( (fd = accept( sd, (struct sockaddr *)&senderAddr, &ic )) != -1 )
		{
			fdptr = (int *)malloc( sizeof(int) );
			*fdptr = fd;					// pointers are not the same size as ints any more.
			if ( pthread_create( &tid, &kernel_attr, client_session_thread, fdptr ) != 0 )
			{
				printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
				return 0;
			}
			else
			{
				continue;
			}
		}
		close( sd );
		return 0;
	}
}
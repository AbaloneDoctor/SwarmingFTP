#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>
#include <ifaddrs.h>


#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1024

//0: no comments, 1: all comments, 2: C<->S control messages
int comments = 1; 
int testInput = 0;
//flags for when a keyword is detected
int fileFlag = 0, serverFlag = 0, portFlag = 0, userFlag = 0, passwordFlag = 0,
	activeFlag = 0, modeFlag = 0, logfileFlag = 0, swarmFlag = 0, numbytesFlag = 0;
FILE *fi;

int help () 
{
	printf( "\nHELP\n\n-v or --version\nPrints the name of the application, the version number (in this case the version has to be 0.1), the author, and exits, returning 0.\n\n[-f file] or [--file file]\nSpecifies the file to download.\n\n[-s hostname] or [--server hostname]\nSpecifies the server to download the file from.\n\n[-p port] or [--port port]\nSpecifies the port to be used when contacting the server. (default value: 21).\n\n[-n user] or [--username user]\nUses the username user when logging into the FTP server (default value: anonymous).\n\n[-P password] or [--password password]\nUses the password password when logging into the FTP server (default value: user@localhost.localnet).\n\n[-a or --active]\nForces active behavior (the server opens the data connection to the client) (default behavior: passive behavior).\n\n[-m mode] or [--mode mode]\nSpecifies the mode to be used for the transfer (ASCII or binary) (default value: binary).\n\n[-l logfile] or [--log logfile]\nLogs all the FTP commands exchanged with the server and the corresponding replies to file logfile. If the filename is \"-\" then the commands are printed to the standard output.\n\n" );		
	return 0;
}

int version()
{
	printf( "\nVERSION\n\nApplication: MFTP\nVersion: 0.1\nAuthor: Spencer Pao\n\n" );
	return 0;
}


//client to server printf
int StoCPrintf( char recv[] )
{

	if( logfileFlag == 1 )
	{
		if( comments == 1 || comments == 2 )printf( "S->C: %s", recv );	
		fprintf( fi, "S->C: %s", recv );
	}	
	else 
	{
		printf( "S->C: %s", recv );
	}

}

int CtoSPrintf( char recv[] )
{

	if( logfileFlag == 1 )
	{
		if( comments == 1 || comments == 2 )printf( "C->S: %s", recv );	
		fprintf( fi, "C->S: %s", recv );
	}	
	else 
	{
		printf( "C->S: %s", recv );
	}

}

int main (int argc, char *argv[])
{
	char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE];
	char filename[MAXLINE], hostname[MAXLINE], port[MAXLINE], user[MAXLINE], 
		password[MAXLINE], mode[MAXLINE], logfile[MAXLINE], swarmconfigfile[MAXLINE];
	//default values:	
	strcpy( port, "21" );
	strcpy( user, "anonymous" );
	strcpy( password, "user@localhost.localnet" );
	strcpy( mode, "binary" );
	strcpy( logfile, "-" );
	//mode: activeFlag: 0 = passive, 1 = active.
	//write: log?
	

	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
 	if (sock < 0) 
	{
		printf("ERROR. Unable to open socket.");
		return -1;
	}	

	//when mftp is run without parameters; defaults to --help
	if( argc == 1 ) 
	{
		help();
		return 0;
	}	

	if( comments == 1 ) printf( "Num Arguments: %d\n", argc );


	//runs through all parameters if there is one or more, and reacts according to keywords
	for ( int i = 1 ; i < argc ; i++ )
	{	
		//debugging printouts
		if( comments == 1 ) printf( "%d: %s\n", i, argv[i] );
		if( comments == 2 ) 
		{
			if( strcmp( argv[i], "test" ) == 0) printf( "TEST FOUND on line %d\n", i );
		}	
		
		//When strings are matched, calls respective functions
		if( strcmp( argv[i], "-h" ) == 0 || strcmp( argv[i], "--help" ) == 0 ) 
		{
			help();
			return 0;
		}

		if( strcmp( argv[i], "-v" ) == 0 || strcmp( argv[i], "--version" ) == 0 ) 
		{
			version();
			return 0;
		}
		
		//checks if keyword requries argument. If no proper argument is given, exit.
		//Will exit if next word is a keyword, or if is empty
		if( 
		strcmp( argv[i], "-f" ) == 0 || strcmp( argv[i], "--file" ) == 0 ||
		strcmp( argv[i], "-s" ) == 0 || strcmp( argv[i], "--server" ) == 0 ||
		strcmp( argv[i], "-p" ) == 0 || strcmp( argv[i], "--port" ) == 0 ||
		strcmp( argv[i], "-n" ) == 0 || strcmp( argv[i], "--username" ) == 0 ||
		strcmp( argv[i], "-P" ) == 0 || strcmp( argv[i], "--password" ) == 0 ||
		strcmp( argv[i], "-m" ) == 0 || strcmp( argv[i], "--mode" ) == 0 ||
		strcmp( argv[i], "-l" ) == 0 || strcmp( argv[i], "--log" ) == 0 ||
		strcmp( argv[i], "-w" ) == 0 || strcmp( argv[i], "--swarm" ) == 0 ) 
		{
			if( i+1 == argc )
			{
				printf( "Error. No argument for %s. \n" , argv[i]);
				return -1;
			}	
			else if( strcmp( argv[i+1], "-h" ) == 0 || strcmp( argv[i+1], "--help" ) == 0 || 
 			strcmp( argv[i+1], "-v" ) == 0 || strcmp( argv[i+1], "--version" ) == 0 || 
			strcmp( argv[i+1], "-s" ) == 0 || strcmp( argv[i+1], "--server" ) == 0 || 
			strcmp( argv[i+1], "-f" ) == 0 || strcmp( argv[i+1], "--file" ) == 0 || 
			strcmp( argv[i+1], "-p" ) == 0 || strcmp( argv[i+1], "--port" ) == 0 || 
			strcmp( argv[i+1], "-n" ) == 0 || strcmp( argv[i+1], "--username" ) == 0 || 
			strcmp( argv[i+1], "-P" ) == 0 || strcmp( argv[i+1], "--password" ) == 0 || 
			strcmp( argv[i+1], "-a" ) == 0 || strcmp( argv[i+1], "--active" ) == 0 || 
			strcmp( argv[i+1], "-m" ) == 0 || strcmp( argv[i+1], "--mode" ) == 0 || 
			strcmp( argv[i+1], "-l" ) == 0 || strcmp( argv[i+1], "--log" ) == 0 ||
			strcmp( argv[i+1], "-w" ) == 0 || strcmp( argv[i+1], "--swarm" ) == 0 || 
			strcmp( argv[i+1], "-b" ) == 0 )
 			{
				printf( "Error. No argument for %s. \n" , argv[i]);
				return -1;
			}
		}
		
		//when keyword is detected, turn on flag, and copy following token as argument
		if( strcmp( argv[i], "-f" ) == 0 || strcmp( argv[i], "--file" ) == 0 ) 
		{
			fileFlag = 1;
			strcpy( filename, argv[i+1] );	
			i++;	
		}

		else if( strcmp( argv[i], "-s" ) == 0 || strcmp( argv[i], "--server" ) == 0 ) 
		{
			serverFlag = 1;
			strcpy( hostname, argv[i+1] );	
			i++;	
		}
		
		else if( strcmp( argv[i], "-p" ) == 0 || strcmp( argv[i], "--port" ) == 0 ) 
		{
			portFlag = 1;
			strcpy( port, argv[i+1] );	
			i++;		
		}

		else if( strcmp( argv[i], "-n" ) == 0 || strcmp( argv[i], "--username" ) == 0 ) 
		{
			userFlag = 1;
			strcpy( user, argv[i+1] );	
			i++;		
		}

		else if( strcmp( argv[i], "-P" ) == 0 || strcmp( argv[i], "--password" ) == 0 ) 
		{
			passwordFlag = 1;
			strcpy( password, argv[i+1] );	
			i++;		
		}

		else if( strcmp( argv[i], "-m" ) == 0 || strcmp( argv[i], "--mode" ) == 0 ) 
		{
			modeFlag = 1;
			strcpy( mode, argv[i+1] );	
			i++;		
		}
		

		else if( strcmp( argv[i], "-l" ) == 0 || strcmp( argv[i], "--logfile" ) == 0 ) 
		{
			logfileFlag = 1;
			strcpy( logfile, argv[i+1] );	
			//if logfile argument is named "-" flag for printout to stdout
			if( strcmp( argv[i+1], "-" ) == 0 ) logfileFlag = 2;
			i++;		
		}


		else if( strcmp( argv[i], "-w" ) == 0 || strcmp( argv[i], "--swarm" ) == 0 ) 
		{
			if( comments == 1 )printf("SWARM BABY SWARM\n");
			swarmFlag = 1;
			strcpy( swarmconfigfile, argv[i+1] );	
			i++;		
		}
	
		else if( strcmp( argv[i], "-b" ) == 0 ) 
		{
			numbytesFlag = 1;
		}
		
		else if( strcmp( argv[i], "-a" ) == 0 || strcmp( argv[i], "--active" ) == 0 ) 
		{
			activeFlag = 1;
		}


		else 
		{
			printf( "Error. Improper argument: %s.  \n", argv[i] );
			return -1;
		}

	}

	if( comments == 1 ) printf( "hostname: %s, filename: %s \n", hostname, filename );
	
	if( numbytesFlag == 1 && swarmFlag == 0 )
	{
		printf( "Error. num-bytes option called without --swarm.  \n" );
		return -1;
	}

	//checks if hostname and filename were specified. If not, exit
	if( strlen( hostname ) == 0 )
	{
		printf( "Error. No host specified.  \n" );
		return -1;
	}

	if( strlen( filename ) == 0 )
	{
		printf( "Error. No file specified.  \n" );
		return -1;
	}
	
	//write: how to log output vs storing
	//replace each printf with a function that either printf's or prints to a file
	if( logfileFlag == 1 )
	{
		//write: real code for recording output
		if( comments == 1 )printf( "Record log into file %s.\n", logfile );
		fi = fopen( logfile, "w" );
	}
	if( logfileFlag == 2 || logfileFlag == 0)
	{
		if( comments == 1 )printf( "Print out log to stdout.\n" );
	}

	struct hostent *server;
	struct sockaddr_in servAddr;

	//sets server to actual address of hostname
	server = gethostbyname( hostname );
	if (server == NULL) 
	{
        	printf( "ERROR. No such host. \n");
        	return(1);
    	}
	else
	{
		if( comments == 1 ) printf( "Server Exists.\n" );
	}	
	
	//Zeros out servAddr
	bzero( ( char * ) &servAddr, sizeof( servAddr ));
	//sets address family to Internet
	servAddr.sin_family = AF_INET;
	//copies bits of server (address) to servAddr
	bcopy( ( char * )server->h_addr, ( char * )&servAddr.sin_addr.s_addr, server->h_length );
	//sets port number. Default: 21
	servAddr.sin_port = htons( atoi ( port ) );

	//Connects to server
	if( connect( sock, ( struct sockaddr * )&servAddr, sizeof( servAddr ) ) < 0) 
	{
		printf("ERROR. Unable to connect to server.\n");
		return(-1);
	}
	else
	{
		if( comments == 1 ) printf( "Connected to %s.\n", hostname );
	}	

	

	//strings for storing send/receive lines to/from server
	char *temp;
	char recv[ MAXLINE ];
	char send[ MAXLINE ];

	//first read
	read( sock, recv, MAXLINE );
	//printf( "S->C: %s", recv );
	StoCPrintf( recv );
	temp = strtok(recv, " ");
    	if ( strcmp( temp, "220" ) != 0 )
    	{
    		printf( "220 reply not received from server.\n" );
    		exit(0);
    	}		

	char sendAddress[ MAXLINE ];

	//REMEMBER TO USE NEWLINE TO DELIMIT WRITES
	strcpy( send, "USER " );
	strcat( send, user );
	strcat( send, "\n" );
	CtoSPrintf( send );
	write( sock, send, strlen( send ) );

	memset( recv, 0, sizeof( recv ) );
	if( comments == 1 ) printf( "second read\n" );
	read( sock, recv, MAXLINE );
	StoCPrintf( recv );
		
	memset( send, 0, sizeof( send ) );
	strcpy( send, "PASS " );
	strcat( send, password );
	strcat( send, "\n" );
	CtoSPrintf( send );
	write( sock, send, strlen( send ) );


	if( comments == 1 ) printf( "third read\n" );
	//scan through each character until you find an integer+space which ends third S->C message
	int endOfMessage = 0;	
	int firstStoC = 0;
	while( endOfMessage == 0 )
	{ 
		//integerFlag keeps track of when an integer is read. If a space follows, end loop.
		int integerFlag = 0;
		//first line needs to include "S->C: ". This makes sure this is only printed on the first line.
		char StoC[ MAXLINE ];
		if( firstStoC == 0 )strcpy( StoC, "S->C: " );		
		else strcpy( StoC, "" );
		firstStoC++;

		memset( recv, 0, sizeof( recv ) );
		read( sock, recv, MAXLINE );

		for( int i = 0; i < strlen( recv ) ; i++ )
		{
			if( comments == -1 ) printf ("end of message\n" );
			if( isspace( recv[i] ) && isdigit( recv[ i-1 ] ) ) endOfMessage = 1;
		}
		
		if( logfileFlag != 1 )printf( "%s%s", StoC, recv );
		else if ( logfileFlag == 1 )
		{
			if( comments == 1 || comments == 2)printf( "%s%s", StoC, recv );
			fprintf( fi, "%s%s", StoC, recv );
		}	
	}	
	
	/*
	memset( send, 0, sizeof( send ) );
	strcpy( send, "PASV" );
	//strcat( send, port );
	strcat( send, "\n" );
	CtoSPrintf( send );
	write( sock, send, strlen( send ) );
	
	memset( recv, 0, sizeof( recv ) );
	read( sock, recv, MAXLINE );
	StoCPrintf( recv );	
	*/

	//int a = 15;
	//printf("0x%x\n", a );

	if( testInput == 1 )
	{
		printf( "Input command\n" );
		
	

		char input[ 20 ];
		char input2[ 20 ];
		char input3[ 20 ];
		int count;		

		scanf( "%s" , input);
		count = atoi( input );
		//printf( "%d \n", count );
		if( count == 1 )
		{
		scanf( "%s" , input );
		strcpy( send, input );
		}
		else if ( count == 2 )
		{
		scanf( "%s %s", input, input2 );
		memset( send, 0, sizeof( send ) );
		strcpy( send, input );
		strcat( send, " " );
		strcat( send, input2 );
		}
		else if ( count == 3 )
		{
		scanf( "%s %s %s", input, input3 );
		memset( send, 0, sizeof( send ) );
		strcpy( send, input );
		strcat( send, " " );
		strcat( send, input2 );
		strcat( send, " " );
		strcat( send, input3 );
		}
		strcat( send, "\n" );

		
		CtoSPrintf( send );
		write( sock, send, strlen( send ) );
	
		memset( recv, 0, sizeof( recv ) );
		read( sock, recv, MAXLINE );
		StoCPrintf( recv );
	}

	//write: code for setting port number
	if( portFlag == 1 )
	{
		if( comments == 1 ) printf( "Setting port\n" );
	}
	
	//if in default passive mode.
	if( activeFlag == 0 )
	{
		if( comments == 1 ) printf ("Passive mode\n" );

		//sends passive command
		memset( send, 0, sizeof( send ) );
		strcpy( send, "PASV\n" );
		CtoSPrintf( send );
		write( sock, send, strlen( send ) );
	
		memset( recv, 0, sizeof( recv ) );
		read( sock, recv, MAXLINE );
		StoCPrintf( recv );	
		
		//parses server response message for IP addr and port num
		int recordNumFlag = 0;
		char addr[ 20 ];
		char serverPort[ 10 ];
		char serverPort2[ 10 ];
		//j incrementor for addr char array			
		int j = 0;
		//k incrementor for port char array part 1
		int k = 0;
		//l incrementor for port char array part 2
		int l = 0;
		int commas = 0;
		for( int i = 0; i < strlen( recv ) ; i++ )
		{

			if( recv[i] == ')' ) recordNumFlag = 0;
			if( recordNumFlag == 1  )
			{

				char temp;
				temp = recv[i];
				if( recv[i] == ',' ) 
				{
					if ( commas < 3 )addr[j] = '.';
					//printf( "commas: %d\n", commas );
					commas++;
				}
				if( recv[i] != ',' && commas <= 3) addr[j] = temp;
				
				if( recv[i] != ',' && commas == 4 ) 
				{
					serverPort[k] = temp;
					k++;
				}
				if( recv[i] != ',' && commas == 5 )
				{
					serverPort2[l] = temp;
					l++;
				}
				j++;
			}
			if( recv[i] == '(' ) recordNumFlag = 1;
		}


		if( comments == 1 ) printf( "addr: %s\n", addr );
		if( comments == 1 ) printf( "port: %s\n", serverPort );		
		if( comments == 1 ) printf( "port: %s\n", serverPort2 );		
		
		char realPort[ 10 ];
		int portPart1 = atoi( serverPort );
		int portPart2 = atoi( serverPort2 );
		portPart1 = portPart1 * 256;
		portPart1 = portPart1 + portPart2;
		if( comments == 1 ) printf( "real port: %d\n", portPart1 );

		//create new socket for data connection
		int ctosSocket;
		ctosSocket = socket( AF_INET, SOCK_STREAM, 0 );
		if( ctosSocket < 0 ) 
		{
			printf( "ERROR. Could not open socket. \n" );
		}

		struct sockaddr_in connectionAddr;
		struct hostent *dataConnection;	

		dataConnection = gethostbyname( addr );
		if (server == NULL) 
		{
			printf( "ERROR. No such host. \n");
			return(1);
	    	}
		else
		{
			if( comments == 1 ) printf( "Server Exists.\n" );
		}	
		//clears out connectionAddr
		bzero( ( char *)&connectionAddr, sizeof( connectionAddr ) );
		//sets family type to internet
		connectionAddr.sin_family = AF_INET;
		//copy addr to socket
		bcopy( ( char * )dataConnection->h_addr, 
			( char * )&connectionAddr.sin_addr.s_addr, dataConnection->h_length );
		//set port number
		//connectionAddr.sin_port = htons( atoi( serverPort ) );
		connectionAddr.sin_port = htons( portPart1 );
		//tries to connect	
		if( comments == 1 )printf( "Attempting connect. \n" );	
		if( connect( ctosSocket, ( struct sockaddr * )&connectionAddr, 
			sizeof( connectionAddr )) < 0) 
        	{	
			printf( "ERROR. Can't connect to server on data connection.\n" );
			return 1;
		}
		else
		{
			if( comments == 1 ) printf( "Connected to %s.\n", addr );
		}	
	
	}

	//memset( recv, 0, sizeof( recv ) );
	//read( sock, recv, MAXLINE );
	//StoCPrintf( recv );

/*
	//Code for port
	struct ifaddrs *ifaddr;
	getifaddrs(&ifaddr);

	memset( send, 0, sizeof( send ) );
	strcpy( send, "PORT " );
	strcat( send, filename );
	strcat( send, "\n" );
	CtoSPrintf( send );
	write( sock, send, strlen( send ) );
	
	memset( recv, 0, sizeof( recv ) );
	read( sock, recv, MAXLINE );
	StoCPrintf( recv );
*/


	memset( send, 0, sizeof( send ) );
	strcpy( send, "RETR " );
	strcat( send, filename );
	strcat( send, "\n" );
	CtoSPrintf( send );
	write( sock, send, strlen( send ) );

//FIX THIS SHIT
	char buffer[ MAXLINE ];
	int size;
	recv(sock, buffer, sizeof(buffer), 0);
	if(!size)
	{
		printf("No such file on the remote directory\n\n");
	}


	memset( recv, 0, sizeof( recv ) );
	read( sock, recv, MAXLINE );
	StoCPrintf( recv );




	if( logfileFlag == 1 ) fclose( fi );
	return 0;
	
}

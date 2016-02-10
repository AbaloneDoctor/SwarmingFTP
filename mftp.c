#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#define MAXLINE 1024

//0: no comments, 1: all comments, 2: specific comments etc
int comments = 1; 
int fileFlag = 0, hostnameFlag = 0, portFlag = 0;

int help () 
{
	printf( "\nHELP\n\n-v or --version\nPrints the name of the application, the version number (in this case the version has to be 0.1), the author, and exits, returning 0.\n\n[-f file] or [--file file]\nSpecifies the file to download.\n\n[-s hostname] or [--server hostname]\nSpecifies the server to download the file from.\n\n[-p port] or [--port port]\nSpecifies the port to be used when contacting the server. (default value: 21).\n\n[-n user] or [--username user]\nUses the username user when logging into the FTP server (default value: anonymous).\n\n[-P password] or [--password password]\nUses the password password when logging into the FTP server (default value: user@localhost.localnet).\n\n-a or --active\nForces active behavior (the server opens the data connection to the client) (default behavior: passive behavior).\n\n[-m mode] or [--mode mode]\nSpecifies the mode to be used for the transfer (ASCII or binary) (default value: binary).\n\n[-l logfile] or [--log logfile]\nLogs all the FTP commands exchanged with the server and the corresponding replies to file logfile. If the filename is \"-\" then the commands are printed to the standard output.\n\n" );		
	return 0;
}

int version()
{
	printf( "\nVERSION\n\nApplication: MFTP\nVersion: 0.1\nAuthor: Spencer Pao\n\n" );
	return 0;
}

int main (int argc, char *argv[])
{
	char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE];
	//when mftp is run without parameters; defaults to --help
	if( argc == 1 ) 
	{
		help();
		return 0;
	}	

	if( comments == 1 ) printf( "Num Arguments: %d\n", argc );
		
	if( argc < 1 ) strcpy( arg1, argv[1] );
	if( argc < 2 ) strcpy( arg2, argv[2] );

	//use while loop to go through argv[i] from i=0 to last one (check if each one argv[i] exist)?
	//check each argv against keywords, and if keyword accepts argument: check argv[i+1] is an
	//appropriate argument
	//for each keyword that is found, set the flag 


	

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

		if( strcmp( argv[i], "-f" ) == 0 || strcmp( argv[i], "--file" ) == 0 ) 
		{
			//write code: if argv[i+1] == (one of our keywords)
			//exit and give error "invalide --file argument"
			//else convert next word into filename
			version();
			return 0;
		}
	}

			

	//if you detect one of argv[x] is -p or --port, check that argv[x+1] is a port number
	//return error if not

}

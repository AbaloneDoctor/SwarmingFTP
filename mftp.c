#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#define MAXLINE 1024

//0: no comments, 1: all comments, 2: specific comments etc
int comments = 1; 
//flags for when a keyword is detected
int fileFlag = 0, serverFlag = 0, portFlag = 0, userFlag = 0, passwordFlag = 0,
	activeFlag = 0, modeFlag = 0, logfileFlag = 0, swarmFlag = 0;

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

int main (int argc, char *argv[])
{
	char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE];
	char filename[MAXLINE], hostname[MAXLINE], port[MAXLINE], user[MAXLINE], 
		password[MAXLINE], mode[MAXLINE], logfile[MAXLINE], swarmconfigfile[MAXLINE];
	//default port is 21.	
	strcpy( port, "21" );
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
				printf( "No argument for %s. Exiting.\n" , argv[i]);
				return 0;
			}	
			else if( strcmp( argv[i+1], "-h" ) == 0 || strcmp( argv[i+1], "--help" ) == 0 ||  				strcmp( argv[i+1], "-v" ) == 0 || strcmp( argv[i+1], "--version" ) == 0 || 
			strcmp( argv[i+1], "-s" ) == 0 || strcmp( argv[i+1], "--server" ) == 0 || 
			strcmp( argv[i+1], "-f" ) == 0 || strcmp( argv[i+1], "--file" ) == 0 || 
			strcmp( argv[i+1], "-p" ) == 0 || strcmp( argv[i+1], "--port" ) == 0 || 
			strcmp( argv[i+1], "-n" ) == 0 || strcmp( argv[i+1], "--username" ) == 0 || 
			strcmp( argv[i+1], "-P" ) == 0 || strcmp( argv[i+1], "--password" ) == 0 || 
			strcmp( argv[i+1], "-a" ) == 0 || strcmp( argv[i+1], "--active" ) == 0 || 
			strcmp( argv[i+1], "-m" ) == 0 || strcmp( argv[i+1], "--mode" ) == 0 || 
			strcmp( argv[i+1], "-l" ) == 0 || strcmp( argv[i+1], "--log" ) == 0 ||
			strcmp( argv[i+1], "-w" ) == 0 || strcmp( argv[i+1], "--swarm" ) == 0 )
 			{
				printf( "No argument for %s. Exiting.\n" , argv[i]);
				return 0;
			}
		}
		
		//when keyword is detected, turn on flag, and copy following token as argument
		if( strcmp( argv[i], "-f" ) == 0 || strcmp( argv[i], "--file" ) == 0 ) 
		{
			fileFlag = 1;
			strcpy( filename, argv[i+1] );	
			i++;	
		}

		if( strcmp( argv[i], "-s" ) == 0 || strcmp( argv[i], "--server" ) == 0 ) 
		{
			serverFlag = 1;
			strcpy( hostname, argv[i+1] );	
			i++;	
		}
		
		if( strcmp( argv[i], "-p" ) == 0 || strcmp( argv[i], "--port" ) == 0 ) 
		{
			portFlag = 1;
			strcpy( port, argv[i+1] );	
			i++;		
		}

		if( strcmp( argv[i], "-n" ) == 0 || strcmp( argv[i], "--username" ) == 0 ) 
		{
			userFlag = 1;
			strcpy( user, argv[i+1] );	
			i++;		
		}

		if( strcmp( argv[i], "-P" ) == 0 || strcmp( argv[i], "--password" ) == 0 ) 
		{
			passwordFlag = 1;
			strcpy( password, argv[i+1] );	
			i++;		
		}

		if( strcmp( argv[i], "-m" ) == 0 || strcmp( argv[i], "--mode" ) == 0 ) 
		{
			modeFlag = 1;
			strcpy( mode, argv[i+1] );	
			i++;		
		}
		

		if( strcmp( argv[i], "-l" ) == 0 || strcmp( argv[i], "--logfile" ) == 0 ) 
		{
			logfileFlag = 1;
			strcpy( logfile, argv[i+1] );	
			i++;		
		}


		if( strcmp( argv[i], "-w" ) == 0 || strcmp( argv[i], "--swarm" ) == 0 ) 
		{
			if( comments == 1 )printf("SWARM BABY SWARM\n");
			swarmFlag = 1;
			strcpy( swarmconfigfile, argv[i+1] );	
			i++;		
		}


	}

			


}

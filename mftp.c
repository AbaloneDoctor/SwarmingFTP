#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#define MAXLINE 1024

int main (int argc, char *argv[])
{
	char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE];
	printf("Num Arguments: %d", argc);
	strcpy(arg1, argv[1]);
	return 0;

}

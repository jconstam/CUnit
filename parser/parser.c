#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"

int main( int argc, char* argv[ ] )
{
	char* rootFolder = NULL;
	
	rootFolder = parseCommandLineArgs( argc, argv );
	if( rootFolder == NULL )
	{
		printf( "Error parsing arguments\n" );
		exit( -1 );
	}
	
	cleanCommandLineArgs( rootFolder );
	
	return 0;
}

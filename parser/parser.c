#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"

int main( int argc, char* argv[ ] )
{
	PARSER_ARGS* args = parseCommandLineArgs( argc, argv );
	if( args == NULL )
	{
		printf( "Error parsing arguments\n" );
		exit( -1 );
	}
	
	cleanCommandLineArgs( args );
	
	return 0;
}

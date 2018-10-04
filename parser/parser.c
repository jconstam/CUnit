#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"
#include "parser_findfile.h"

//static void failAndExit( PARSER_ARGS* args, FILE_ENTRY* fileList )

int main( int argc, char* argv[ ] )
{
	PARSER_ARGS* args = parseCommandLineArgs( argc, argv );
	if( args == NULL )
	{
		exit( -1 );
	}
	
	FILE_ENTRY* fileList = findAllCFiles( args );
	if( fileList == NULL )
	{
		exit( -1 );
	}
	
	cleanCommandLineArgs( args );
	
	return 0;
}

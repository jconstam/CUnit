#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"
#include "parser_findfile.h"

static void cleanupAndExit( bool successful, PARSER_ARGS* args, FILE_LIST* fileList )
{
	cleanCommandLineArgs( args );
	cleanupFileList( fileList );
	
	exit( successful ? 0 : -1 );
}

int main( int argc, char* argv[ ] )
{
	PARSER_ARGS* args = parseCommandLineArgs( argc, argv );
	if( args == NULL )
	{
		cleanupAndExit( false, args, NULL );
	}
	
	FILE_LIST* fileList = findAllCFiles( args );
	if( fileList == NULL )
	{
		cleanupAndExit( false, args, fileList );
	}
	
	cleanupAndExit( true, args, fileList );
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"
#include "parser_findfile.h"
#include "parser_readfile.h"
#include "parser_output.h"

static void cleanupAndExit( bool successful, PARSER_ARGS* args, FILE_LIST* fileList, TEST_LIST* testList )
{
	cleanCommandLineArgs( args );
	cleanupFileList( fileList );
	cleanupTestList( testList );
	
	exit( successful ? 0 : -1 );
}

int main( int argc, char* argv[ ] )
{
	PARSER_ARGS* args = parseCommandLineArgs( argc, argv );
	if( args == NULL )
	{
		cleanupAndExit( false, args, NULL, NULL );
	}
	
	FILE_LIST* fileList = findAllCFiles( args );
	if( fileList == NULL )
	{
		cleanupAndExit( false, args, fileList, NULL );
	}
	
	TEST_LIST* testList = parseFiles( args, fileList );
	if( testList == NULL )
	{
		cleanupAndExit( false, args, fileList, testList );
	}
	
	if( ! createOutputFile( args, testList ) )
	{
		cleanupAndExit( false, args, fileList, testList );
	}
	
	cleanupAndExit( true, args, fileList, testList );
}

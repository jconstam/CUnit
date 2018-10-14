#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "parser_args.h"

PARSER_ARGS* parseCommandLineArgs( int argc, char* argv[ ] )
{
	int opt = 0;
	PARSER_ARGS* args = calloc( 1U, sizeof( PARSER_ARGS ) );
	if( args != NULL )
	{
		args->verbose = false;
		
		while( opt >= 0 )
		{
			opt = getopt( argc, argv, "vr:o:" );
			switch( opt )
			{
				case( 'v' ):
					args->verbose = true;
					break;
				case( 'r' ):
					( void ) realpath( optarg, args->rootFolder );
					break;
				case( 'o' ):
					( void ) realpath( optarg, args->outputFile );
					break;
				case( '?' ):
					printf( "Received unknown command line parameter\n" );
					
					free( args );
					args = NULL;
					
					opt = -1;
					break;
				default:
					opt = -1;
					break;
			}
		}
	}
	
	if( ( args != NULL ) && ( args->verbose == true ) )
	{
		printf( "Command line arguments:\n" );
		printf( "\tVerbose:      %s\n", ( args->verbose ? "YES" : "NO" ) );
		printf( "\tRoot Folder:  %s\n", args->rootFolder );
		printf( "\tOutput File:  %s\n", args->outputFile );
	}
	
	return args;
}

void cleanCommandLineArgs( PARSER_ARGS* args )
{
	free( args );
}
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "parser_args.h"

PARSER_ARGS* parseCommandLineArgs( int argc, char* argv[ ] )
{
	int opt = 0;
	PARSER_ARGS* args = calloc( 1U, sizeof( PARSER_ARGS ) );
	if( args != NULL )
	{
		while( opt >= 0 )
		{
			opt = getopt( argc, argv, "r:" );
			switch( opt )
			{
				case( 'r' ):
					args.rootFolder = calloc( strlen( optarg ), sizeof( char ) );
					if( args.rootFolder != NULL )
					{
						strcpy( args.rootFolder, optarg );	
					}
					break;
				case( '?' ):
					opt = -1;
					if( args != NULL )
					{
						free( args.rootFolder );
					}
					free( args );
					args = NULL;
					break;
				default:
					opt = -1;
					break;
			}
		}
	}
	
	return args;
}

void cleanCommandLineArgs( PARSER_ARGS* args )
{
	if( args != NULL )
	{
		free( args.rootFolder );
	}
	free( args );
}
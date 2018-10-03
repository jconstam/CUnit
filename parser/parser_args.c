#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* parseCommandLineArgs( int argc, char* argv[ ] )
{
	int opt = 0;
	char* rootFolder = NULL;

	while( opt >= 0 )
	{
		opt = getopt( argc, argv, "r:" );
		switch( opt )
		{
			case( 'r' ):
				rootFolder = calloc( strlen( optarg ), sizeof( char ) );
				if( rootFolder != NULL )
				{
					strcpy( rootFolder, optarg );	
				}
				break;
			default:
				opt = -1;
				break;
		}
	}
	
	return rootFolder;
}

void cleanCommandLineArgs( char* rootFolder )
{
	free( rootFolder );
}
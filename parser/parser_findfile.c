#include <stdio.h>
#include <dirent.h> 
#include <string.h>

#include "parser_findfile.h"

#define ALLOC_SIZE		( 100U )

static void parseDir( PARSER_ARGS* args, char* rootPath, char* currPath, FILE_LIST* fileList )
{
	char fullPath[ PATH_MAX ];
	char subPath[ PATH_MAX ];
	struct dirent* dirEntry;
	uint32_t fileNameLen;
	
	memset( fullPath, 0, PATH_MAX );
	strcat( fullPath, rootPath );
	if( currPath[ 0 ] != '\0' )
	{
		strcat( fullPath, "/" );
		strcat( fullPath, currPath );
	}
	
	if( args->verbose )
	{
		printf( "Searching for C files in %s...\n", fullPath );
	}
	
	DIR* dirHandle = opendir( fullPath ); 
	if( dirHandle != NULL )
	{
		while( true )
		{
			if( fileList->count >= fileList->allocated )
			{
				fileList->allocated += ALLOC_SIZE;
				fileList->files = realloc( fileList->files, fileList->allocated * sizeof( FILE_NAME ) );
			}
				
			dirEntry = readdir( dirHandle );
			if( dirEntry == NULL )
			{
				break;
			}
			else if( ( strcmp( dirEntry->d_name, "." ) == 0 ) || ( strcmp( dirEntry->d_name, ".." ) == 0 ) )
			{
				continue;
			}
			else
			{
				if( dirEntry->d_type == DT_REG )
				{
					fileNameLen = strlen( dirEntry->d_name );
					if( ( fileNameLen >= 2U ) && ( dirEntry->d_name[ fileNameLen - 2U ] == '.' ) && ( dirEntry->d_name[ fileNameLen - 1U ] == 'c' ) )
					{
						memset( fileList->files[ fileList->count ].fileName, 0, PATH_MAX );
						strcat( fileList->files[ fileList->count ].fileName, fullPath );
						strcat( fileList->files[ fileList->count ].fileName, "/" );
						strcat( fileList->files[ fileList->count ].fileName, dirEntry->d_name );
						
						if( args->verbose )
						{
							printf( "\t%s\n", fileList->files[ fileList->count ].fileName );
						}
						
						fileList->count++;
					}
				}
				else if( dirEntry->d_type == DT_DIR )
				{
					if( currPath[ 0 ] != '\0' )
					{
						strcat( subPath, currPath );
						strcat( subPath, "/" );
					}
					strcat( subPath, dirEntry->d_name );
					
					parseDir( args, rootPath, subPath, fileList );
				}
			}
		}
	
		closedir( dirHandle );
	}
}

FILE_LIST* findAllCFiles( PARSER_ARGS* args )
{
	char currPath[ PATH_MAX ];
	FILE_LIST* fileList = calloc( 1U, sizeof( FILE_LIST ) );
	if( fileList == NULL )
	{
		return NULL;
	}
	else
	{
		fileList->count = 0U;
		fileList->allocated = 0U;
		memset( currPath, 0, PATH_MAX );
		
		parseDir( args, args->rootFolder, currPath, fileList );
	}
	
	return fileList;
}

void cleanupFileList( FILE_LIST* fileList )
{
	if( fileList != NULL )
	{
		free( fileList->files );
	}
	free( fileList );
}
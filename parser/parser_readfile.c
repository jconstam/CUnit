#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "parser_readfile.h"

#define ALLOC_SIZE				( 100U )

#define TEST_FILE_DECL			"CUNIT_TEST_FILE"
#define TEST_FUNC_DECL			"CUNIT_TEST"

#define MAX_NUM_PARAMS			( 20U )
#define MAX_PARAM_LENGTH		( 256U )
#define PARAM_ALLOC_COUNT		( 10U )

#define PARAM_COUNT_DECL		( 4U )
#define DECL_PARAM_IDX_NAME		( 0U )
#define DECL_PARAM_IDX_SETUP	( 2U )
#define DECL_PARAM_IDX_CLEANUP	( 3U )

#define FUNC_PARAM_IDX_MODULE	( 0U )
#define FUNC_PARAM_IDX_NAME		( 1U )

typedef struct
{
	uint32_t	allocated;
	uint32_t	charCount;
	
	char*		contents;
} FILE_DATA;

static uint32_t getFileSize( FILE* fileHandle )
{
	uint32_t fileSize;
	
	fseek( fileHandle, 0U, SEEK_END );
	
	fileSize = ftell( fileHandle );

	fseek( fileHandle, 0U, SEEK_SET );
	
	return fileSize;
}

static FILE_DATA* preParseFile( FILE* fileHandle )
{
	bool inSinglecomment = false;
	bool inMultiComment = false;
	bool possibleComment = false;
	char prevChar = '\0';
	char currChar = '\0';
	
	FILE_DATA* currentFile = calloc( 1U, sizeof( FILE_DATA ) );
	if( currentFile == NULL )
	{
		return NULL;
	}
	
	while( true )
	{
		if( currentFile->charCount >= currentFile->allocated )
		{
			currentFile->allocated += ALLOC_SIZE * sizeof( char );
			currentFile->contents = realloc( currentFile->contents, currentFile->allocated );
			if( currentFile->contents == NULL )
			{
				free( currentFile );
				return NULL;
			}
		}
		
		currChar = fgetc( fileHandle );
		if( currChar == EOF )
		{
			break;
		}
		
		if( inMultiComment )
		{
			if( ( prevChar == '*' ) && ( currChar == '/' ) )
			{
				inMultiComment = false;
			}
		}
		else if( inSinglecomment )
		{
			if( currChar == '\n' )
			{
				inSinglecomment = false;
			}
		}
		else if( ( prevChar == '/' ) && ( currChar == '*' ) )
		{
			inMultiComment = true;
			possibleComment = false;
		}
		else if( ( prevChar == '/' ) && ( currChar == '/' ) )
		{
			inSinglecomment = true;
			possibleComment = false;
		}
		else if( possibleComment )
		{
			currentFile->contents[ currentFile->charCount++ ] = prevChar;
			currentFile->contents[ currentFile->charCount++ ] = currChar;
			
			possibleComment = false;
		}
		else if( currChar == '/' )
		{
			possibleComment = true;
		}
		else if( ! isspace( currChar ) )
		{
			currentFile->contents[ currentFile->charCount++ ] = currChar;
		}
		
		prevChar = currChar;
	}
	
	return currentFile;
}

typedef struct
{
	bool		hasName;
	bool 		hasTooManyParams;
	
	uint32_t	offset;
	
	uint32_t 	parameterCount;
	char 		parameters[ MAX_NUM_PARAMS ][ MAX_PARAM_LENGTH ];
} LINE_CONTENTS;

static LINE_CONTENTS* parseLine( const char* name, char* text )
{
	LINE_CONTENTS* lineData = calloc( 1U, sizeof( LINE_CONTENTS ) );
	if( lineData == NULL )
	{
		return NULL;
	}

	lineData->hasName = false;
	lineData->hasTooManyParams = false;
	lineData->parameterCount = 0U;
	
	char* nameLocation = strstr( text, name );
	if( nameLocation == NULL )
	{
		return lineData;
	}
	
	lineData->hasName = true;	
	
	char* start = nameLocation + strlen( name ) + 1;
	char* end = strstr( start, ")" );
	lineData->offset = end - text;
	
	char* declaration = calloc( end - start, sizeof( char ) );
	if( declaration == NULL )
	{
		free( lineData );
		return NULL;
	}
	
	strncpy( declaration, start, end - start );
	
	const char delim[ 2 ] = ",";
	char* token = strtok( declaration, delim );
	while( token != NULL )
	{
		if( lineData->parameterCount >= MAX_NUM_PARAMS )
		{
			lineData->hasTooManyParams = true;
			free( declaration );
			return NULL;
		}
		
		strcpy( lineData->parameters[ lineData->parameterCount++ ], token );
		
		token = strtok( NULL, delim );
	}
	
	free( declaration );
	
	return lineData;
}

static TEST_MODULE* parseFile( PARSER_ARGS* args, FILE_DATA* preParsedFile, char* fileName )
{
	bool successful = false;
	LINE_CONTENTS* currTestFunc = NULL;
	LINE_CONTENTS* testFileDecl = parseLine( TEST_FILE_DECL, preParsedFile->contents );
	if( testFileDecl == NULL )
	{
		if( args->verbose )
		{
			printf( "\tError processing file\n" );
		}
		
		goto parseFileCleanup;
	}
	else if( ! testFileDecl->hasName )
	{
		if( args->verbose )
		{
			printf( "\tCould not locate unit test file declaration (CUNIT_TEST_FILE)\n" );
		}
		
		goto parseFileCleanup;
	}
	else if( testFileDecl->parameterCount != PARAM_COUNT_DECL )
	{
		if( args->verbose )
		{
			printf( "\tUnit test file declaration has too many parameters - got %u, expected %u\n", testFileDecl->parameterCount, PARAM_COUNT_DECL );
		}
		
		goto parseFileCleanup;
	}
	
	TEST_MODULE* module = calloc( 1U, sizeof( TEST_MODULE ) );
	if( module == NULL )
	{
		if( args->verbose )
		{
			printf( "\tError creating module\n" );
		}
		
		goto parseFileCleanup;
	}
	
	strcpy( module->fileName, fileName );
	strcpy( module->moduleName, testFileDecl->parameters[ DECL_PARAM_IDX_NAME ] );
	strcpy( module->setupName, testFileDecl->parameters[ DECL_PARAM_IDX_SETUP ] );
	strcpy( module->cleanupName, testFileDecl->parameters[ DECL_PARAM_IDX_CLEANUP ] );
	
	if( args->verbose )
	{
		printf( "\tFound module %s in file %s with setup function %s and cleanup function %s\n", module->moduleName, module->fileName, module->setupName, module->cleanupName );
	}
	
	char* remainingFileData = preParsedFile->contents + testFileDecl->offset;
	
	while( true )
	{
		if( module->functionCount >= module->functionsAllocated )
		{
			module->functionsAllocated += ALLOC_SIZE * sizeof( TEST_FUNC );
			module->functions = realloc( module->functions, module->functionsAllocated * sizeof( TEST_FUNC ) );
			if( module->functions == NULL )
			{
				if( args->verbose )
				{
					printf( "\tError parsing module functions\n" );
					goto parseFileCleanup;
				}
			}
		}
		
		currTestFunc = parseLine( TEST_FUNC_DECL, remainingFileData );
		if( ! currTestFunc->hasName )
		{
			if( args->verbose )
			{
				printf( "\tDone parsing module %s in file %s\n", module->moduleName, module->fileName );
			}
			
			break;
		}
		else if( currTestFunc->parameterCount >= PARAM_COUNT_DECL )
		{
			if( args->verbose )
			{
				printf( "\tUnit test function declaration %s in module %s has too many parameters (%d)\n", currTestFunc->parameters[ FUNC_PARAM_IDX_NAME ], currTestFunc->parameters[ FUNC_PARAM_IDX_MODULE ], MAX_NUM_PARAMS );
			}
			
			goto parseFileCleanup;
		}
		else if( strcmp( currTestFunc->parameters[ FUNC_PARAM_IDX_MODULE ], module->moduleName ) != 0 )
		{
			if( args->verbose )
			{
				printf( "\tUnit test function declaration %s is for module %s, expected module %s\n", currTestFunc->parameters[ FUNC_PARAM_IDX_NAME ], currTestFunc->parameters[ FUNC_PARAM_IDX_MODULE ], module->moduleName );
			}
			
			goto parseFileCleanup;
		}
		else
		{
			strcpy( module->functions[ module->functionCount ].testName, currTestFunc->parameters[ FUNC_PARAM_IDX_NAME ] );
			if( args->verbose )
			{
				printf( "\tFound unit test function %s in module %s\n", module->functions[ module->functionCount ].testName, module->moduleName );
			}
			module->functionCount++;
		}
		
		remainingFileData += currTestFunc->offset;
		free( currTestFunc );
		currTestFunc = NULL;
	}
	
	successful = true;
	
parseFileCleanup:
	if( testFileDecl != NULL )
	{
		free( testFileDecl );
	}
	
	if( currTestFunc != NULL )
	{
		free( currTestFunc );
	}
	
	if( ( ! successful ) && ( module != NULL ) )
	{
		if( module->functions != NULL )
		{
			free( module->functions );
		}
		free( module );
		module = NULL;
	}

	return module;
}

TEST_LIST* parseFiles( PARSER_ARGS* args, FILE_LIST* fileList )
{
	uint32_t fileIndex;
	
	for( fileIndex = 0U; fileIndex < fileList->count; fileIndex++ )
	{
		uint32_t fileSize = 0U;
		FILE* fileHandle = NULL;
		FILE_DATA* preParsedFile = NULL;
		TEST_MODULE* moduleData = NULL;
	
		if( args->verbose )
		{
			printf( "Scanning test file %s\n", fileList->files[ fileIndex ].fileName );
		}
		
		fileHandle = fopen( fileList->files[ fileIndex ].fileName, "r" );
		if( fileHandle == NULL )
		{
			if( args->verbose )
			{
				printf( "\tCould not open file\n" );
			}
			goto cleanupParseFiles;
		}
		
		fileSize = getFileSize( fileHandle );
		if( fileSize == 0U )
		{
			if( args->verbose )
			{
				printf( "\tFile is 0 bytes\n" );
			}
			goto cleanupParseFiles;
		}
		
		preParsedFile = preParseFile( fileHandle );
		if( preParsedFile == NULL )
		{
			if( args->verbose )
			{
				printf( "\tFailure on first-pass parse\n" );
			}
			goto cleanupParseFiles;
		}
		
		moduleData = parseFile( args, preParsedFile, fileList->files[ fileIndex ].fileName );
		if( moduleData == NULL )
		{
			if( args->verbose )
			{
				printf( "\tFailure on final parse\n" );
			}
			goto cleanupParseFiles;
		}
		
		continue;
		
cleanupParseFiles:
		if( fileHandle != NULL )
		{
			fclose( fileHandle );
		}
		
		if( preParsedFile != NULL )
		{
			if( preParsedFile->contents != NULL )
			{
				free( preParsedFile->contents );
			}
			free( preParsedFile );
		}
		
		if( moduleData != NULL )
		{
			if( moduleData->functions != NULL )
			{
				free( moduleData->functions );
			}
			free( moduleData );
		}
	}
	
	return NULL;
}



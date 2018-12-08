#include <stdio.h>
#include <string.h>

#include "parser_output.h"

static void printHeader( FILE* fileHandle, char* title )
{
	const int totalWidth = 50;
	
	fprintf( fileHandle, "// %s ", title );
	
	int i;
	for( i = strlen( title ) + 4; i < totalWidth; i++ )
	{
		fputc( '=', fileHandle );
	}
	fputc( '\n', fileHandle );
}

static void printTestFuncName( FILE* fileHandle, char* moduleName, char* funcName )
{
	fprintf( fileHandle, "CUNIT_TEST__%s__%s", moduleName, funcName );
}

static void printVoidFuncExtern( FILE* fileHandle, char* funcName )
{
	fprintf( fileHandle, "extern void %s( void );\n", funcName );
}

static void printTestFuncExtern( FILE* fileHandle, char* moduleName, char* funcName )
{
	fprintf( fileHandle, "extern void " );
	printTestFuncName( fileHandle, moduleName, funcName );
	fprintf( fileHandle, "( bool* );\n" );
}

bool createOutputFile( PARSER_ARGS* args, TEST_LIST* tests )
{
	uint32_t moduleIndex;
	uint32_t testIndex;
	
	FILE* fileHandle = fopen( args->outputFile, "w" );
	if( fileHandle == NULL )
	{
		if( args->verbose )
		{
			printf( "Could not open output file %s\n", args->outputFile );
		}
		return false;
	}
	
	fprintf( fileHandle, "#include \"ctest.h\"\n" );
	fprintf( fileHandle, "\n" );
	printHeader( fileHandle, "PREPROCESSOR DEFINES" );
	fprintf( fileHandle, "#define MODULE_COUNT ( %uU )\n", tests->moduleCount );
	fprintf( fileHandle, "\n" );
	printHeader( fileHandle, "TYPEDEFS" );
	fprintf( fileHandle, "typedef void ( *setupFunc )( void );\n" );
	fprintf( fileHandle, "typedef void ( *cleanupFunc )( void );\n" );
	fprintf( fileHandle, "typedef void ( *testFunc )( bool* );\n" );
	fprintf( fileHandle, "\n" );
	fprintf( fileHandle, "typedef struct\n" );
	fprintf( fileHandle, "{\n" );
	fprintf( fileHandle, "\tconst char* moduleName;\n" );
	fprintf( fileHandle, "\n" );
	fprintf( fileHandle, "\tsetupFunc setup;\n" );
	fprintf( fileHandle, "\tcleanupFunc cleanup;\n" );
	fprintf( fileHandle, "\n" );
	fprintf( fileHandle, "\tuint32_t functionCount;\n" );
	fprintf( fileHandle, "\ttestFunc* functions;\n" );
	fprintf( fileHandle, "} TEST_MODULE;\n" );
	fprintf( fileHandle, "\n" );
	
	for( moduleIndex = 0U; moduleIndex < tests->moduleCount; moduleIndex++ )
	{
		TEST_MODULE* currModule = &( tests->modules[ moduleIndex ] );
		
		printHeader( fileHandle, currModule->moduleName );
		printVoidFuncExtern( fileHandle, currModule->setupName );
		printVoidFuncExtern( fileHandle, currModule->cleanupName );
		fprintf( fileHandle, "\n" );
		
		for( testIndex = 0U; testIndex < currModule->functionCount; testIndex++ )
		{
			printTestFuncExtern( fileHandle, currModule->moduleName, currModule->functions[ testIndex ].testName );
		}
		fprintf( fileHandle, "\n" );
		
		/*fprintf( fileHandle, "TestFunc %s_functions[ %d ]\n", currModule->moduleName, currModule->functionCount );
		fprintf( fileHandle, "{\n" );
		for( testIndex = 0U; testIndex < currModule->functionCount; testIndex++ )
		{
			fprintf( fileHandle, "\tCUNIT_TEST__%s__%s,\n", currModule->moduleName, currModule->functions[ testIndex ].testName );
		}
		fprintf( fileHandle, "}\n" );
		fprintf( fileHandle, "\n" );*/
	}
	
	printHeader( fileHandle, "TEST DATA" );
	fprintf( fileHandle, "TEST_MODULE tests[ MODULE_COUNT ] = \n" );
	fprintf( fileHandle, "{\n" );
	for( moduleIndex = 0U; moduleIndex < tests->moduleCount; moduleIndex++ )
	{
		TEST_MODULE* currModule = &( tests->modules[ moduleIndex ] );
		fprintf( fileHandle, "\t{\n" );
		fprintf( fileHandle, "\t\t%s,\n", currModule->moduleName );
		fprintf( fileHandle, "\t\t%s,\n", currModule->setupName );
		fprintf( fileHandle, "\t\t%s,\n", currModule->cleanupName );
		fprintf( fileHandle, "\t\t%uU,\n", currModule->functionCount );
		fprintf( fileHandle, "\t\t{\n" );
		for( testIndex = 0U; testIndex < currModule->functionCount; testIndex++ )
		{
			fprintf( fileHandle, "\t\t\t" );
			printTestFuncName( fileHandle, currModule->moduleName, currModule->functions[ testIndex ].testName );
			fprintf( fileHandle, "( bool* ),\n" );
		}
		fprintf( fileHandle, "\t\t}\n" );
		fprintf( fileHandle, "\t},\n" );
	}
	fprintf( fileHandle, "};\n" );
	fprintf( fileHandle, "\n" );
	
	printHeader( fileHandle, "MAIN" );
	fprintf( fileHandle, "int main( int argc, char* argv[ ] )\n" );
	fprintf( fileHandle, "{\n" );
	fprintf( fileHandle, "\tbool pass;\n" );
	fprintf( fileHandle, "\tuint32_t moduleIndex;\n" );
	fprintf( fileHandle, "\tuint32_t testIndex;\n" );
	fprintf( fileHandle, "\n" );
	
	fprintf( fileHandle, "\tprintf( \"CUnit Test\\n\" );\n" );
	fprintf( fileHandle, "\t\n" );
	fprintf( fileHandle, "\tfor( moduleIndex = 0U; moduleIndex < MODULE_COUNT; moduleIndex++ )\n" );
	fprintf( fileHandle, "\t{\n" );
	fprintf( fileHandle, "\t\tTEST_MODULE* currModule = &( tests[ moduleIndex ] );\n" );
	fprintf( fileHandle, "\n" );
	fprintf( fileHandle, "\t\tprintf( \"Module %%d: %%s\\n\", moduleIndex, currModule->moduleName );\n" );
	fprintf( fileHandle, "\t\tfor( testIndex = 0U; testIndex < currModule->functionCount; testIndex++ )\n" );
	fprintf( fileHandle, "\t\t{\n" );
	fprintf( fileHandle, "\t\t\tcurrModule->setup( );\n" );
	fprintf( fileHandle, "\t\t\tcurrModule->functions[ testIndex ]( &( pass ) );\n" );
	fprintf( fileHandle, "\t\t\tcurrModule->cleanup( );\n" );
	fprintf( fileHandle, "\t\t\tif( !pass )\n" );
	fprintf( fileHandle, "\t\t\t\tbreak;\n" );
	fprintf( fileHandle, "\t\t\t{\n" );
	fprintf( fileHandle, "\t\t\t}\n" );
	fprintf( fileHandle, "\t\t}\n" );
	fprintf( fileHandle, "\t}\n" );
	/*for( moduleIndex = 0U; moduleIndex < tests->moduleCount; moduleIndex++ )
	{
		TEST_MODULE* currModule = &( tests->modules[ moduleIndex ] );
		
		fprintf( fileHandle, "\tprintf( \"Module: %s\\n\" );\n", currModule->moduleName );
		fprintf( fileHandle, "\tprintf( \"\\tRunning Setup...\" );\n" );
		fprintf( fileHandle, "\t%s( );\n", currModule->setupName );
		fprintf( fileHandle, "\tprintf( \"Done.\\n\" );\n" );
		fprintf( fileHandle, "\n" );
		
		for( testIndex = 0U; testIndex < currModule->functionCount; testIndex++ )
		{
			fprintf( fileHandle, "\tprintf( \"\\tRunning Test %d (%s)...\" );\n", testIndex, currModule->functions[ testIndex ].testName );
			fprintf( fileHandle, "\tCUNIT_TEST__%s__%s( &( pass ) );\n", currModule->moduleName, currModule->functions[ testIndex ].testName );
			fprintf( fileHandle, "\tprintf( \"Done.\\n\" );\n" );
		}
		
		fprintf( fileHandle, "\n" );
		fprintf( fileHandle, "\tprintf( \"\\tRunning Cleanup...\" );\n" );
		fprintf( fileHandle, "\t%s( );\n", currModule->cleanupName );
		fprintf( fileHandle, "\tprintf( \"Done.\\n\" );\n" );
		if( moduleIndex != tests->moduleCount - 1U )
		{
			fprintf( fileHandle, "\n\n" );
		}
	}*/
	fprintf( fileHandle, "\n" );
	fprintf( fileHandle, "\treturn ( pass ? 0 : 1 );\n" );
	fprintf( fileHandle, "}\n" );
	
	fclose( fileHandle );
	
	return true;
}
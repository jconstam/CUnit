#include <stdlib.h>

typedef struct
{
	char*	rootFolder;
} PARSER_ARGS;

PARSER_ARGS* parseCommandLineArgs( int, char*[ ] );
void cleanCommandLineArgs( PARSER_ARGS* );

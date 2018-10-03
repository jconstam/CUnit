#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct
{
	bool	verbose;
	char	rootFolder[ PATH_MAX ];
} PARSER_ARGS;

PARSER_ARGS* parseCommandLineArgs( int, char*[ ] );
void cleanCommandLineArgs( PARSER_ARGS* );

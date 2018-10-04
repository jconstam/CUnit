#ifndef PARSER_FINDFILE_H__
#define PARSER_FINDFILE_H__

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "parser_args.h"

typedef struct
{
	char	fileName[ PATH_MAX ];
} FILE_NAME;

typedef struct
{
	uint32_t			allocated;
	uint32_t			count;
	
	FILE_NAME*			files;
} FILE_LIST;

FILE_LIST* findAllCFiles( PARSER_ARGS* );
void cleanupFileList( FILE_LIST* fileList );

#endif

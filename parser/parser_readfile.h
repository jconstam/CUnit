#ifndef PARSER_READFILE_H__
#define PARSER_READFILE_H__

#include "parser_args.h"
#include "parser_findfile.h"

#define TEST_NAME_MAX_LEN		( 100U )

typedef struct
{
	char			testName[ TEST_NAME_MAX_LEN ];
} TEST_FUNC;

typedef struct
{
	char			fileName[ PATH_MAX ];
	char			moduleName[ TEST_NAME_MAX_LEN ];
	char			setupName[ TEST_NAME_MAX_LEN ];
	char			cleanupName[ TEST_NAME_MAX_LEN ];
	
	uint32_t		functionsAllocated;
	uint32_t		functionCount;
	TEST_FUNC*		functions;
} TEST_MODULE;

typedef struct
{
	uint32_t		modulesAllocated;
	uint32_t		moduleCount;
	
	TEST_MODULE*	modules;
} TEST_LIST;

TEST_LIST* parseFiles( PARSER_ARGS*, FILE_LIST* );

#endif

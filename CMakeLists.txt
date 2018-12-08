cmake_minimum_required( VERSION 2.6 )

set( CMAKE_VERBOSE_MAKEFILE off )

set( PARSER_FOLDER "parser" )

project( ${PARSER_PROJECT} )

set( PARSER_SOURCES 
	${PARSER_FOLDER}/parser.c
	${PARSER_FOLDER}/parser_args.c
	${PARSER_FOLDER}/parser_findfile.c
	${PARSER_FOLDER}/parser_readfile.c
	${PARSER_FOLDER}/parser_output.c
	)
	
include_directories( ${PROJECT_SOURCE_DIR}/${PARSER_FOLDER} )

add_executable( ${PARSER_PROJECT} ${PARSER_SOURCES} )
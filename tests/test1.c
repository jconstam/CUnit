#include "cunit.h"

#include CUNIT_TEST_FILE( ONE, one.c, One_Setup, One_Cleanup )

/* 
	THIS IS A FULL COMMENT THAT SHOULD BE IGNORED
*/

// This one too

void One_Setup( void )	// More comments to ignore
{
	printf( "Setup for Test One" );
}

void One_Cleanup( void )
{
	printf( "Cleanup for Test One" );
}

CUNIT_TEST( ONE, checkTrue )
{
	EXPECT_EQ( true, One_returnTrue( ) );
}

CUNIT_TEST( ONE, checkFalse )
{
	EXPECT_EQ( false, One_returnFalse( ) );
}

/* You should also ignore CUNIT_TEST in a comment */

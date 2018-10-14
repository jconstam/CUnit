#include "cunit.h"

#include CUNIT_TEST_FILE( TWO, two.c, Test_Two_Setup, TesT_Two_Cleanup )

void Test_Two_Setup( void )
{
	printf( "Setup for Test Two" );
}

void Test_Two_Cleanup( void )
{
	printf( "Cleanup for Test Two" );
}

CUNIT_TEST( TWO, checkTrue )
{
	EXPECT_EQ( true, true );
}

CUNIT_TEST( TWO, checkFalse )
{
	EXPECT_EQ( false, false );
}

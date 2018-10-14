#ifndef CUNIT_H__
#define CUNIT_H__

#define CUNIT_TEST_FILE( moduleName, fileName, setupFunc, cleanupFunc )		#fileName

#define CUNIT_TEST( moduleName, testName )									void CUNIT_TEST_##testName( boolean* passed )

#define CUNIT_TEST( one, condition, two )									if( !( ( expected ) condition ( actual ) ) ) { printf( "\tFAILED: %d %s %d.\n", ( expected ), #condition, ( actual ) ); *( passed ) = false; }

#define EXPECT_EQ( expected, actual )										CUNIT_TEST( ( expected ), ==, ( actual ) )
#define EXPECT_NE( expected, actual )										CUNIT_TEST( ( expected ), !=, ( actual ) )

#endif
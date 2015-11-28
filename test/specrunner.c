#include <test/specrunner.h>
#include <test/example_tests.h>
#include <timer.h>
#include <stdio.h>

void SpecRunner_test() {
	TestFramework_print( "Romble Unit Test Report" );
	TestFramework_print( "-----------------------" );

	u32 tickStart = getTime( FALSE );

	// Your unit tests go here
	// Format:
	// TestFramework_execute( <test case group name/description>, <number of test cases>, <test case array> )
	// TODO: Is there a way to get rid of the second argument? it's annoying
	TestFramework_execute( "Addition (example test cases)", 2, ExampleTests );

	u32 ticksElapsed = getTime( FALSE ) - tickStart;

	char resultTime[ 100 ] = { 0 };
	sprintf( resultTime, "Tests finished. Time elapsed (ms): %d", ticksElapsed );
	TestFramework_print( resultTime );
	TestFramework_print( "Press [RESET] to re-execute tests." );
}

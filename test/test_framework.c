#include <test/test_framework.h>
#include <types.h>
#include <string.h>
#include <stdio.h>
#include <kdebug.h>
#include <stdarg.h>

const char TestFramework_ANSI_YELLOW[] = { 0x1B, '[', '3', '3', 'm', 0 };
const char TestFramework_ANSI_GREEN[] = { 0x1B, '[', '3', '2', 'm', 0 };
const char TestFramework_ANSI_RED[] = { 0x1B, '[', '3', '1', 'm', 0 };
const char TestFramework_ANSI_RESET[] = { 0x1B, '[', '0', 'm', 0 };

void TestFramework_execute( const char* descriptionText, u8 length, const TestFramework_TestCaseDefinition* cases ) {
	u8 step;

	TestFramework_print( descriptionText );

	for( step = 0; step != length; step++ ) {
		TestFramework_TestCaseDefinition testCase = cases[ step ];

		TestFramework_report(
			testCase.description,
			testCase.test()
		);
	}
}

/**
 * This implementation uses KDebug_Alert in SGDK for use in Gens Kmod emulator
 *
 * Implementations running on real hardware may use an output to serial port
 *
 * @param		{const char*}							text
 */
void TestFramework_print( const char* text ) {
	KDebug_Alert( text );
}

void TestFramework_sprint( const char* format, ... ) {
	va_list args;
	va_start( args, format );

	char string[ 100 ];
	vsprintf( string, format, args );
	TestFramework_print( string );

	va_end( args );
}

void TestFramework_report( const char* text, TestFramework_TestResult pass ) {

	// static allocation: easier, less messy, less error-prone
	char resultString[ 300 ] = { 0 };

	// Begin by inserting indentation
	strcpy( resultString, TestFramework_INDENTATION );

	// Insert ANSI Color and pass/fail status
	switch( pass ) {
		case TestFramework_TestResult_TEST_PASS:
			//strcat( resultString, TestFramework_ANSI_GREEN );
			strcat( resultString, TestFramework_PASS_MESSAGE );
			break;
		case TestFramework_TestResult_TEST_FAIL:
			//strcat( resultString, TestFramework_ANSI_RED );
			strcat( resultString, TestFramework_FAIL_MESSAGE );
			break;
		default:
			// This case means you wrote your test case wrong, or
			// memory is being corrupted in your unit being tested.
			//strcat( resultString, TestFramework_ANSI_YELLOW );
			strcat( resultString, TestFramework_UNDF_MESSAGE );
	}

	// Reset color
	//strcat( resultString, TestFramework_ANSI_RESET );
	// Separator
	strcat( resultString, TestFramework_SEPARATOR );
	// Test case text
	strcat( resultString, text );

	// Log it!
	TestFramework_print( resultString );
}

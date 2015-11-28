#include <test/test_framework.h>
#include <types.h>
#include <string.h>
#include <kdebug.h>

void TestFramework_execute( const char* descriptionText, u8 length, TestFramework_TestCaseDefinition* cases ) {
	u8 step;

	TestFramework_report( descriptionText, TestFramework_TestResult_TEST_DEFINE );

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
 * @param		{TestFramework_TestResult}				pass		Whether or not this test case passed. If not NULL,
 * 																	text will be output to console indented one-in.
 */
void TestFramework_report( const char* text, TestFramework_TestResult pass ) {

	if( pass == TestFramework_TestResult_TEST_DEFINE ) {
		KDebug_Alert( text );
	} else {
		// static allocation: easier, less messy, less error-prone
		char resultString[ 300 ] = { 0 };

		// Begin by inserting indentation
		strcpy( resultString, TestFramework_INDENTATION );

		// Insert ANSI Color and pass/fail status
		switch( pass ) {
			case TestFramework_TestResult_TEST_PASS:
				strcat( resultString, TestFramework_ANSI_GREEN );
				strcat( resultString, TestFramework_PASS_MESSAGE );
				break;
			case TestFramework_TestResult_TEST_FAIL:
				strcat( resultString, TestFramework_ANSI_RED );
				strcat( resultString, TestFramework_FAIL_MESSAGE );
				break;
			default:
				// This case means you wrote your test case wrong, or
				// memory is being corrupted in your unit being tested.
				strcat( resultString, TestFramework_ANSI_YELLOW );
				strcat( resultString, TestFramework_UNDF_MESSAGE );
		}

		// Reset color
		strcat( resultString, TestFramework_ANSI_RESET );
		// Separator
		strcat( resultString, TestFramework_SEPARATOR );
		// Test case text
		strcat( resultString, text );

		// Log it!
		KDebug_Alert( resultString );
	}
}

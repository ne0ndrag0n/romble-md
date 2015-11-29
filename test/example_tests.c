#include <test/example_tests.h>
#include <test/test_framework.h>
#include <types.h>


const TestFramework_TestCaseDefinition ExampleTests[] = {
	{
		"Should add 2+2 properly",
		ExampleTests_twoPlusTwo
	},
	{
		"This test case always fails",
		ExampleTests_alwaysFails
	}
};

TestFramework_TestResult ExampleTests_twoPlusTwo() {
	u8 result = 2 + 2;

	if( result == 4 ) {
		return TestFramework_TestResult_TEST_PASS;
	} else {
		return TestFramework_TestResult_TEST_FAIL;
	}
}

TestFramework_TestResult ExampleTests_alwaysFails() {
	TestFramework_EXPECT( TRUE == FALSE );

	return TestFramework_TestResult_TEST_PASS;

onFail:
	return TestFramework_TestResult_TEST_FAIL;
}

#ifndef	LL_TESTS
#define LL_TESTS

#include 	<test/test_framework.h>
#include	<res/globals.h>

extern const TestFramework_TestCaseDefinition LinkedListTests[];

TestFramework_TestResult LinkedListTests_verifyLinkedListCreated();
TestFramework_TestResult LinkedListTests_verifyInsertEnd();
TestFramework_TestResult LinkedListTests_findData();

bool LinkedListTests_searchPredicateStringDivisibleBy2( void* data );
bool LinkedListTests_alwaysFalse( void* data );

#endif

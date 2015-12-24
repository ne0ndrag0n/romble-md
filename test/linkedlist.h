#ifndef	LL_TESTS
#define LL_TESTS

#include 	<test/test_framework.h>
#include	<res/globals.h>

extern const TestFramework_TestCaseDefinition LinkedListTests[];

TestFramework_TestResult LinkedListTests_verifyLinkedListCreated();
TestFramework_TestResult LinkedListTests_verifyInsertEnd();
TestFramework_TestResult LinkedListTests_findData();
TestFramework_TestResult LinkedListTests_removeItemsTest1();
TestFramework_TestResult LinkedListTests_removeItemsTest2();
TestFramework_TestResult LinkedListTests_removeSingleNode();
TestFramework_TestResult LinkedListTests_removeFirstItem();

bool LinkedListTests_searchPredicateStringDivisibleBy2( void* data );
bool LinkedListTests_alwaysFalse( void* data );
bool LinkedListTests_searchFor2s( void* data );

#endif

#include <test/bit_trie.h>
#include <test/test_framework.h>
#include <bit_trie.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>

const TestFramework_TestCaseDefinition BitTrieTests[] = {
	{
		"Should properly construct a bit trie node with four children set to NULL",
		BitTrieTests_verifyBitTrieCreated
	},
	{
		"Should properly construct a leaf node",
		BitTrieTests_verifyLeafCreated
	},
	{
		"Should insert 0 correctly",
		BitTrieTests_insertZeroCorrectly
	},
	{
		"Should insert 255 correctly",
		BitTrieTests_insert255Correctly
	},
	{
		"Should insert 27 correctly (00 01 10 11)",
		BitTrieTests_insert27Correctly
	}
};

TestFramework_TestResult BitTrieTests_verifyBitTrieCreated() {
	BitwiseTrieNode* test;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	if( test->children == NULL ) {
		return TestFramework_TestResult_TEST_FAIL;
	}

	for( u8 i = 0; i != 4; i++ ) {
		if( test->children[ i ] != NULL ) {
			return TestFramework_TestResult_TEST_FAIL;
		}
	}

	free( test );
	return TestFramework_TestResult_TEST_PASS;
}

TestFramework_TestResult BitTrieTests_verifyLeafCreated() {
	BitwiseTrieNode* test;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, TRUE );

	u8 result = test->data == NULL ? TestFramework_TestResult_TEST_PASS: TestFramework_TestResult_TEST_FAIL;

	free( test );
	return result;
}

TestFramework_TestResult BitTrieTests_insertZeroCorrectly() {
	TestFramework_TestResult testResult;
	BitwiseTrieNode* test;
	BitwiseTrieNode* next;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 0, testString );

	next = test;

	for( u8 level = 0; level != 4; level++ ) {
		TestFramework_EXPECT( next->children[ 0 ] != NULL, "first child to not be null" );
		TestFramework_EXPECT( next->children[ 1 ] == NULL, "second child to be null" );
		TestFramework_EXPECT( next->children[ 2 ] == NULL, "third child to be null" );
		TestFramework_EXPECT( next->children[ 3 ] == NULL, "fourth child to be null" );

		next = next->children[ 0 ];
	}

	TestFramework_EXPECT( next->data == testString, "data address to match original address" );

	// All expectations passed
	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	// To ensure the integrity of other tests, we can't free anything unless we know
	// the state of the unit test (to prevent double-free)
	// Leaving some memory hanging around is less likely to impact other unit tests
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		free( next );
		free( test->children[ 0 ]->children[ 0 ]->children[ 0 ] );
		free( test->children[ 0 ]->children[ 0 ] );
		free( test->children[ 0 ] );
		free( test );
	}
	return testResult;
}

TestFramework_TestResult BitTrieTests_insert255Correctly() {
	TestFramework_TestResult testResult;
	BitwiseTrieNode* test;
	BitwiseTrieNode* next;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 255, testString );

	next = test;

	for( u8 level = 0; level != 4; level++ ) {
		TestFramework_EXPECT( next->children[ 0 ] == NULL, "first child to be null" );
		TestFramework_EXPECT( next->children[ 1 ] == NULL, "second child to be null" );
		TestFramework_EXPECT( next->children[ 2 ] == NULL, "third child to be null" );
		TestFramework_EXPECT( next->children[ 3 ] != NULL, "fourth child to not be null" );

		next = next->children[ 3 ];
	}

	TestFramework_EXPECT( next->data == testString, "data address to match original address" );

	// All expectations passed
	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	// To ensure the integrity of other tests, we can't free anything unless we know
	// the state of the unit test (to prevent double-free)
	// Leaving some memory hanging around is less likely to impact other unit tests
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		free( next );
		free( test->children[ 3 ]->children[ 3 ]->children[ 3 ] );
		free( test->children[ 3 ]->children[ 3 ] );
		free( test->children[ 3 ] );
		free( test );
	}
	return testResult;
}

TestFramework_TestResult BitTrieTests_insert27Correctly() {
	TestFramework_TestResult testResult;
	BitwiseTrieNode* test;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 27, testString );

	// First level
	TestFramework_EXPECT( test->children[ 0 ] == NULL, "lv 1 first child to be null" );
	TestFramework_EXPECT( test->children[ 1 ] == NULL, "lv 1 second child to be null" );
	TestFramework_EXPECT( test->children[ 2 ] == NULL, "lv 1 third child to be null" );
	TestFramework_EXPECT( test->children[ 3 ] != NULL, "lv 1 fourth child to not be null" );

	// Second level
	TestFramework_EXPECT( test->children[ 3 ]->children[ 0 ] == NULL, "lv 2 first child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 1 ] == NULL, "lv 2 second child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ] != NULL, "lv 2 third child to not be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 3 ] == NULL, "lv 2 fourth child to be null" );

	// Third level
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 0 ] == NULL, "lv 3 first child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ] != NULL, "lv 3 second child to not be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 2 ] == NULL, "lv 3 third child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 3 ] == NULL, "lv 3 fourth child to be null" );

	// Fourth level
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 0 ] != NULL, "lv 4 first child to not be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 1 ] == NULL, "lv 4 second child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 2 ] == NULL, "lv 4 third child to be null" );
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 3 ] == NULL, "lv 4 fourth child to be null" );

	// Data
	TestFramework_EXPECT( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 0 ]->data == testString, "data address to match original address" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		free( test->children[ 3 ]->children[ 2 ]->children[ 1 ]->children[ 0 ] );
		free( test->children[ 3 ]->children[ 2 ]->children[ 1 ] );
		free( test->children[ 3 ]->children[ 2 ] );
		free( test->children[ 3 ] );
		free( test );
	}
	return testResult;
}

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

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 0, testString );

	for( u8 level = 0; level != 4; level++ ) {
		TestFramework_EXPECT( test->children[ 0 ] != NULL, "first child to not be null" );
		TestFramework_EXPECT( test->children[ 1 ] == NULL, "second child to be null" );
		TestFramework_EXPECT( test->children[ 2 ] == NULL, "third child to be null" );
		TestFramework_EXPECT( test->children[ 3 ] == NULL, "fourth child to be null" );

		test = test->children[ 0 ];
	}

	TestFramework_EXPECT( test->data == testString, "data address to match original address" );

	// All expectations passed
	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	free( test );
	return testResult;
}

TestFramework_TestResult BitTrieTests_insert255Correctly() {
	TestFramework_TestResult testResult;
	BitwiseTrieNode* test;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 255, testString );

	for( u8 level = 0; level != 4; level++ ) {
		TestFramework_EXPECT( test->children[ 0 ] == NULL, "first child to be null" );
		TestFramework_EXPECT( test->children[ 1 ] == NULL, "second child to be null" );
		TestFramework_EXPECT( test->children[ 2 ] == NULL, "third child to be null" );
		TestFramework_EXPECT( test->children[ 3 ] != NULL, "fourth child to be null" );

		test = test->children[ 3 ];
	}

	TestFramework_EXPECT( test->data == testString, "data address to match original address" );

	// All expectations passed
	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	free( test );
	return testResult;
}

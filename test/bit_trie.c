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
	BitwiseTrieNode* test;

	test = calloc( 1, sizeof( BitwiseTrieNode ) );
	BitwiseTrieNode_ctor( test, FALSE );

	char* testString = "Test String";
	BitwiseTrieNode_insert( test, 0, testString );

	for( u8 level = 0; level != 4; level++ ) {
		TestFramework_EXPECT( test->children[ 0 ] != NULL );
		TestFramework_EXPECT( test->children[ 1 ] == NULL );
		TestFramework_EXPECT( test->children[ 2 ] == NULL );
		TestFramework_EXPECT( test->children[ 3 ] == NULL );

		test = test->children[ 0 ];
	}

	TestFramework_EXPECT( test->data == testString );

	free( test );
	return TestFramework_TestResult_TEST_PASS;

onFail:
	free( test );
	return TestFramework_TestResult_TEST_FAIL;
}

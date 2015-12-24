#include <test/linkedlist.h>
#include <test/test_framework.h>
#include <linkedlist.h>
#include <types.h>
#include <stdlib.h>
#include <string.h>

const TestFramework_TestCaseDefinition LinkedListTests[] = {
	{
		"Should be able to create and hold a single linked list node",
		LinkedListTests_verifyLinkedListCreated
	},
	{
		"Should be able to insert two items at the end of the list each time",
		LinkedListTests_verifyLinkedListCreated
	},
	{
		"Should properly find a piece of data based on a predicate function",
		LinkedListTests_findData
	},
	{
		"Should properly remove several items (first item not removed)",
		LinkedListTests_removeItemsTest1
	},
	{
		"Should properly remove several items (first item removed)",
		LinkedListTests_removeItemsTest2
	},
	{
		"Should free and null a single node that ended up removed",
		LinkedListTests_removeSingleNode
	},
	{
		"Should free the first item and stop properly",
		LinkedListTests_removeFirstItem
	}
};

TestFramework_TestResult LinkedListTests_verifyLinkedListCreated() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	TestFramework_EXPECT( node->data == NULL, "node data after construction to be null" );
	TestFramework_EXPECT( node->next == NULL, "node next after construction to be null" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		// This method is simple enough to just use directly untested
		LinkedListNode_dtor( node );
	}

	return testResult;
}

TestFramework_TestResult LinkedListTests_verifyInsertEnd() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	char* string1 = "This is string 1";
	char* string2 = "This is string 2";
	char* string3 = "This is string 3";

	node->data = string1;

	LinkedListNode_insertEnd( node, string2 );
	LinkedListNode_insertEnd( node, string3 );

	TestFramework_EXPECT( node->data == string1, "root node data to be string1" );
	TestFramework_EXPECT( node->next->data == string2, "second node data to be string2" );
	TestFramework_EXPECT( node->next->next->data == string3, "third node data to be string3" );
	TestFramework_EXPECT( node->next->next->next == NULL, "final node to be null" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		// This method is simple enough to just use directly untested
		LinkedListNode_dtor( node );
	}

	return testResult;
}

TestFramework_TestResult LinkedListTests_findData() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	int int1 = 1;
	int int2 = 2;
	int int3 = 3;

	node->data = &int1;

	LinkedListNode_insertEnd( node, &int2 );
	LinkedListNode_insertEnd( node, &int3 );

	int* ptr = LinkedListNode_findData( node, LinkedListTests_searchPredicateStringDivisibleBy2 );

	TestFramework_EXPECT( ptr == &int2, "returned pointer to be equal to the address of int2" );

	void* ptr2 = LinkedListNode_findData( node, LinkedListTests_alwaysFalse );

	TestFramework_EXPECT( ptr2 == NULL, "returned pointer for invalid data to be NULL" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		// This method is simple enough to just use directly untested
		LinkedListNode_dtor( node );
	}

	return testResult;
}

TestFramework_TestResult LinkedListTests_removeItemsTest1() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	int int1 = 3;
	int int2 = 2;
	int int3 = 3;
	int int4 = 2;
	int int5 = 3;

	node->data = &int1;

	LinkedListNode_insertEnd( node, &int2 );
	LinkedListNode_insertEnd( node, &int3 );
	LinkedListNode_insertEnd( node, &int4 );
	LinkedListNode_insertEnd( node, &int5 );

	LinkedListNode_remove( &node, LinkedListTests_searchFor2s, TRUE );

	TestFramework_EXPECT( node->data == &int1, "first node to be retained" );
	TestFramework_EXPECT( node->next->data == &int3, "second node to now be 3" );
	TestFramework_EXPECT( node->next->next->data == &int5, "third node to now be 3" );
	TestFramework_EXPECT( node->next->next->next == NULL, "last node to be properly terminated." );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		// This method is simple enough to just use directly untested
		LinkedListNode_dtor( node );
	}

	return testResult;
}

TestFramework_TestResult LinkedListTests_removeItemsTest2() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	int int1 = 2;
	int int2 = 3;
	int int3 = 2;
	int int4 = 3;
	int int5 = 2;

	node->data = &int1;

	LinkedListNode_insertEnd( node, &int2 );
	LinkedListNode_insertEnd( node, &int3 );
	LinkedListNode_insertEnd( node, &int4 );
	LinkedListNode_insertEnd( node, &int5 );

	LinkedListNode_remove( &node, LinkedListTests_searchFor2s, TRUE );

	TestFramework_EXPECT( node->data == &int2, "first node to be retained" );
	TestFramework_EXPECT( node->next->data == &int4, "second node to now be 3" );
	TestFramework_EXPECT( node->next->next == NULL, "last node to be properly terminated." );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	if( testResult == TestFramework_TestResult_TEST_PASS ) {
		// This method is simple enough to just use directly untested
		LinkedListNode_dtor( node );
	}

	return testResult;
}

TestFramework_TestResult LinkedListTests_removeSingleNode() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );

	int int1 = 2;

	node->data = &int1;

	LinkedListNode_remove( &node, LinkedListTests_searchFor2s, TRUE );

	TestFramework_EXPECT( node == NULL, "single node removed to be zeroed out" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	return testResult;
}

TestFramework_TestResult LinkedListTests_removeFirstItem() {
	TestFramework_TestResult testResult;
	LinkedListNode* node;

	int int1 = 2;
	int int2 = 3;

	node = calloc( 1, sizeof( LinkedListNode ) );
	TestFramework_EXPECT( node != NULL, "no heap corruption" );
	LinkedListNode_ctor( node );
	node->data = &int1;

	node->next = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( node->next );
	node->next->data = &int2;

	LinkedListNode_remove( &node, LinkedListTests_searchFor2s, FALSE );

	TestFramework_EXPECT( node != NULL, "pointer not to be null" );
	TestFramework_EXPECT( node->data == &int2, "pointer to now be at the first node" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	LinkedListNode_dtor( node );

	return testResult;
}

bool LinkedListTests_searchPredicateStringDivisibleBy2( void* data ) {
	int number = *( ( int* ) data );

	return number % 2 == 0;
}

bool LinkedListTests_alwaysFalse( void* data ) {
	return FALSE;
}

bool LinkedListTests_searchFor2s( void* data ) {
	int number = *( ( int* ) data );

	return number == 2;
}

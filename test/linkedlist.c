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
	}
};

TestFramework_TestResult LinkedListTests_verifyLinkedListCreated() {
	TestFramework_TestResult testResult;


}

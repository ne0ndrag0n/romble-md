#ifndef	BIT_TRIE_TESTS
#define BIT_TRIE_TESTS

#include 	<test/test_framework.h>

extern const TestFramework_TestCaseDefinition BitTrieTests[];

TestFramework_TestResult BitTrieTests_verifyBitTrieCreated();
TestFramework_TestResult BitTrieTests_verifyLeafCreated();
TestFramework_TestResult BitTrieTests_insertZeroCorrectly();
TestFramework_TestResult BitTrieTests_insert255Correctly();
TestFramework_TestResult BitTrieTests_insert27Correctly();

#endif

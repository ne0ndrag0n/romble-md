#ifndef	BIT_TRIE_TESTS
#define BIT_TRIE_TESTS

#include 	<test/test_framework.h>

extern const TestFramework_TestCaseDefinition BitTrieTests[];

// Known problems with memory leaks in the free portions (node->children arrays not being freed)
TestFramework_TestResult BitTrieTests_verifyBitTrieCreated();
TestFramework_TestResult BitTrieTests_verifyLeafCreated();
TestFramework_TestResult BitTrieTests_insertZeroCorrectly();
TestFramework_TestResult BitTrieTests_insert255Correctly();
TestFramework_TestResult BitTrieTests_insert27Correctly();
TestFramework_TestResult BitTrieTests_insertTwoIntegersCorrectly();
TestFramework_TestResult BitTrieTests_deleteOne();
TestFramework_TestResult BitTrieTests_deleteWithMultiple();
TestFramework_TestResult BitTrieTests_deleteNull();

#endif

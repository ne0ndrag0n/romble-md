#ifndef EXAMPLE_TESTS
#define EXAMPLE_TESTS

#include <test/test_framework.h>

// Uses namespace "ExampleTests" and may be included in specrunner as
// "ExampleTests"

TestFramework_TestResult ExampleTests_twoPlusTwo();
TestFramework_TestResult ExampleTests_alwaysFails();

extern const TestFramework_TestCaseDefinition ExampleTests[];

#endif

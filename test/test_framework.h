#ifndef TEST_FRAMEWORK
#define TEST_FRAMEWORK

#define		TestFramework_PASS_MESSAGE	"[PASS]"
#define		TestFramework_FAIL_MESSAGE	"[FAIL]"
#define		TestFramework_UNDF_MESSAGE	"[ ?? ]"
#define		TestFramework_SEPARATOR		" - "
#define		TestFramework_INDENTATION	"   "

#include <res/globals.h>
#include <types.h>

const char TestFramework_ANSI_YELLOW[] = { 0x1B, '[', '3', '3', 'm', 0 };
const char TestFramework_ANSI_GREEN[] = { 0x1B, '[', '3', '2', 'm', 0 };
const char TestFramework_ANSI_RED[] = { 0x1B, '[', '3', '1', 'm', 0 };
const char TestFramework_ANSI_RESET[] = { 0x1B, '[', '0', 'm', 0 };

typedef enum {
	TestFramework_TestResult_TEST_PASS,
	TestFramework_TestResult_TEST_FAIL,
	TestFramework_TestResult_TEST_DEFINE
} TestFramework_TestResult;

typedef TestFramework_TestResult ( *TestFramework_TestCase )();

typedef struct {
	const char* description;
	TestFramework_TestCase test;
} TestFramework_TestCaseDefinition;

void TestFramework_execute( const char* descriptionText, u8 length, TestFramework_TestCaseDefinition* cases );

void TestFramework_report( const char* text, TestFramework_TestResult pass );

#endif

#ifndef TEST_FRAMEWORK
#define TEST_FRAMEWORK

#define		TestFramework_PASS_MESSAGE	"√ [ PASS ]"
#define		TestFramework_FAIL_MESSAGE	"X [ FAIL ]"
#define		TestFramework_UNDF_MESSAGE	"  [ ???? ]"
#define		TestFramework_SEPARATOR		" - "
#define		TestFramework_INDENTATION	"   "

#include <res/globals.h>
#include <types.h>

extern const char TestFramework_ANSI_YELLOW[];
extern const char TestFramework_ANSI_GREEN[];
extern const char TestFramework_ANSI_RED[];
extern const char TestFramework_ANSI_RESET[];

typedef enum {
	TestFramework_TestResult_TEST_PASS,
	TestFramework_TestResult_TEST_FAIL
} TestFramework_TestResult;

typedef TestFramework_TestResult ( *TestFramework_TestCase )();

typedef struct {
	const char* description;
	TestFramework_TestCase test;
} TestFramework_TestCaseDefinition;

void TestFramework_execute( const char* descriptionText, u8 length, const TestFramework_TestCaseDefinition* cases );

void TestFramework_report( const char* text, TestFramework_TestResult pass );
void TestFramework_print( const char* text );

#endif

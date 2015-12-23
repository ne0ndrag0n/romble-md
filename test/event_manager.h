#ifndef EVENT_MANAGER_TESTS
#define EVENT_MANAGER_TESTS

#include <test/test_framework.h>
#include <eventmanager.h>
#include <types.h>

#define 	EventManagerTests_DEMO_EVENT_1		0x1990
#define		EventManagerTests_DEMO_EVENT_2		0x2015

extern const TestFramework_TestCaseDefinition EventManagerTests[];

typedef struct EventManagerTests_ExampleObject {
	u8 one;
	u8 two;
} EventManagerTests_ExampleObject;

TestFramework_TestResult EventManagerTests_verifyCtor();
TestFramework_TestResult EventManagerTests_verifyRegisterSingleEvent();
TestFramework_TestResult EventManagerTests_verifyRegisterTwoOfSameEvent();
TestFramework_TestResult EventManagerTests_verifyRegisterTwoSeparateEvents();

TestFramework_TestResult EventManagerTests_verifyUnregisterOneItem();

void EventManagerTests_eventTarget( void* context, void* payload );
void EventManagerTests_setupMockEvents( EventManager* eventManager, void* instance );
#endif

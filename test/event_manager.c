#include <test/event_manager.h>
#include <test/test_framework.h>
#include <types.h>
#include <res/globals.h>
#include <stdio.h>
#include <stdlib.h>
#include <eventmanager.h>

const TestFramework_TestCaseDefinition EventManagerTests[] = {
	{
		"Should properly initialize an EventManager",
		EventManagerTests_verifyCtor
	},
	{
		"Should properly register one event",
		EventManagerTests_verifyRegisterSingleEvent
	}
};

TestFramework_TestResult EventManagerTests_verifyCtor() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	TestFramework_EXPECT( eventManager->events == NULL, "eventManager->events to be NULL" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyRegisterSingleEvent() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	// Register EventManagerTests_DEMO_EVENT_1 on exampleInstance, using EventManagerTests_eventTarget
	EventManager_registerEvent(
		eventManager,
		exampleInstance,
		EventManagerTests_DEMO_EVENT_1,
		EventManagerTests_eventTarget
	);

	EventManager_RegisteredEvent* event = eventManager->events->data;

	TestFramework_EXPECT( event->eventKey == EventManagerTests_DEMO_EVENT_1, "first registered event to have key EventManagerTests_DEMO_EVENT1" );

	EventManager_EventListener* listener = event->listeners->data;
	TestFramework_EXPECT( listener->instance == exampleInstance, "first listener to have instance exampleInstance" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "first listener to have callback EventManagerTests_eventTarget" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

void EventManagerTests_eventTarget( void* context, void* payload ) {
	EventManagerTests_ExampleObject* this = ( EventManagerTests_ExampleObject* ) context;

	this->one = 0x01;
	if( payload != NULL ) {
		this->two = 0x02;
	}
}

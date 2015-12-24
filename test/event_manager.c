#include <test/event_manager.h>
#include <test/test_framework.h>
#include <types.h>
#include <res/globals.h>
#include <stdio.h>
#include <stdlib.h>
#include <eventmanager.h>
#include <linkedlist.h>

const TestFramework_TestCaseDefinition EventManagerTests[] = {
	{
		"Should properly initialize an EventManager",
		EventManagerTests_verifyCtor
	},
	{
		"Should properly register one event",
		EventManagerTests_verifyRegisterSingleEvent
	},
	{
		"Should properly register two separate instances on one event",
		EventManagerTests_verifyRegisterTwoOfSameEvent
	},
	{
		"Should properly register two separate events",
		EventManagerTests_verifyRegisterTwoSeparateEvents
	},
	{
		"Should properly unregister one event listener",
		EventManagerTests_verifyUnregisterOneItem
	},
	{
		"Should properly remove one event listener from a registered event",
		EventManagerTests_verifyUnregisterOneEventListener
	},
	{
		"Should properly remove an entire registered event, leaving others unharmed (removing first)",
		EventManagerTests_verifyUnregisterEntireEventWithOthers
	},
	{
		"Should properly remove an entire registered event, leaving others unharmed (removing second)",
		EventManagerTests_verifyUnregisterEntireSecondEventWithOthers
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

	TestFramework_EXPECT( event->eventKey == EventManagerTests_DEMO_EVENT_1, "first registered event to have key EventManagerTests_DEMO_EVENT_1" );

	EventManager_EventListener* listener = event->listeners->data;
	TestFramework_EXPECT( listener->instance == exampleInstance, "first listener to have instance exampleInstance" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "first listener to have callback EventManagerTests_eventTarget" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyRegisterTwoOfSameEvent() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );
	EventManagerTests_ExampleObject* exampleInstance2 = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	// Register EventManagerTests_DEMO_EVENT_1 on exampleInstance
	EventManager_registerEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_1, EventManagerTests_eventTarget );
	// Register same event on another instance
	EventManager_registerEvent( eventManager, exampleInstance2, EventManagerTests_DEMO_EVENT_1, EventManagerTests_eventTarget );

	TestFramework_EXPECT( eventManager->events->next == NULL, "not to add another event" );

	EventManager_RegisteredEvent* event = eventManager->events->data;
	TestFramework_EXPECT( event->eventKey == EventManagerTests_DEMO_EVENT_1, "first registered event to have key EventManagerTests_DEMO_EVENT_1" );

	// Check first listener
	EventManager_EventListener* listener = event->listeners->data;
	TestFramework_EXPECT( listener->instance == exampleInstance, "first listener to have instance exampleInstance" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "first listener to have callback EventManagerTests_eventTarget" );

	// Move down and check next listener
	listener = event->listeners->next->data;

	TestFramework_EXPECT( listener->instance == exampleInstance2, "second listener to have instance exampleInstance2" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "second listener to have callback EventManagerTests_eventTarget" );

	TestFramework_EXPECT( event->listeners->next->next == NULL, "there only to be two listeners" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );
	free( exampleInstance2 );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyRegisterTwoSeparateEvents() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	// Register EventManagerTests_DEMO_EVENT_1 on exampleInstance with EventManagerTests_eventTarget
	EventManager_registerEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_1, EventManagerTests_eventTarget );
	// Register EventManagerTests_DEMO_EVENT_2 on exampleInstance with EventManagerTests_eventTarget
	EventManager_registerEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_2, EventManagerTests_eventTarget );

	TestFramework_EXPECT( eventManager->events != NULL, "not to add no events" );
	TestFramework_EXPECT( eventManager->events->next != NULL, "not to add only one event" );
	TestFramework_EXPECT( eventManager->events->next->next == NULL, "to only add two events" );

	// Check first event
	EventManager_RegisteredEvent* event = eventManager->events->data;
	TestFramework_EXPECT( event->eventKey == EventManagerTests_DEMO_EVENT_1, "first registered event to have key EventManagerTests_DEMO_EVENT_1" );
	EventManager_EventListener* listener = event->listeners->data;
	TestFramework_EXPECT( event->listeners->next == NULL, "first listener to have no link to second listener" );
	TestFramework_EXPECT( listener->instance == exampleInstance, "first listener to have instance exampleInstance" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "first listener to have callback EventManagerTests_eventTarget" );

	// Check second event
	event = eventManager->events->next->data;
	TestFramework_EXPECT( event->eventKey == EventManagerTests_DEMO_EVENT_2, "second registered event to have key EventManagerTests_DEMO_EVENT_2" );
	listener = event->listeners->data;
	TestFramework_EXPECT( event->listeners->next == NULL, "first listener in second event to have no link to second listener" );
	TestFramework_EXPECT( listener->instance == exampleInstance, "first listener in second event to have instance exampleInstance" );
	TestFramework_EXPECT( listener->callback == EventManagerTests_eventTarget, "first listener in second event to have callback EventManagerTests_eventTarget" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyUnregisterOneItem() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	// Mock a correct event setup
	EventManagerTests_setupMockEvents( eventManager, exampleInstance );

	// Verify for testing purposes...
	TestFramework_EXPECT( eventManager->events != NULL, "the events, pre-test, to be there" );

	// Remove the event
	EventManager_unregisterEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_1 );

	TestFramework_EXPECT( eventManager->events == NULL, "all events to be properly cleaned up" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyUnregisterOneEventListener() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );
	EventManagerTests_ExampleObject* exampleInstance2 = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	EventManagerTests_setupMockEvents( eventManager, exampleInstance );
	// Add another EventListener to first RegisteredEvent
	EventManager_EventListener* secondListener = calloc( 1, sizeof( EventManager_EventListener ) );
	secondListener->instance = exampleInstance2;
	secondListener->callback = EventManagerTests_eventTarget;

	EventManager_RegisteredEvent* first = eventManager->events->data;
	first->listeners->next = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( first->listeners->next );
	first->listeners->next->data = secondListener;

	EventManager_unregisterEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_1 );

	TestFramework_EXPECT( eventManager->events != NULL, "registered event to still be remaining for instance not removed" );

	// Verify the first registered event only has the listener for exampleInstance2 present
	first = eventManager->events->data;
	EventManager_EventListener* firstListener = first->listeners->data;

	TestFramework_EXPECT( firstListener->instance == exampleInstance2, "remaining event listener to be for exampleInstance2" );
	TestFramework_EXPECT( first->listeners->next == NULL, "remaining event listener to be the only event listener" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );
	free( exampleInstance2 );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyUnregisterEntireEventWithOthers() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	EventManagerTests_setupMockEvents( eventManager, exampleInstance );

	// Add one for DEMO_EVENT_2
	EventManager_RegisteredEvent* secondEvent = calloc( 1, sizeof( EventManager_RegisteredEvent ) );
	secondEvent->eventKey = EventManagerTests_DEMO_EVENT_2;
	secondEvent->listeners = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( secondEvent->listeners );

	EventManager_EventListener* eventListener2 = calloc( 1, sizeof( EventManager_EventListener ) );
	eventListener2->instance = exampleInstance;
	eventListener2->callback = EventManagerTests_eventTarget;

	secondEvent->listeners->data = eventListener2;

	eventManager->events->next = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( eventManager->events->next );
	eventManager->events->next->data = secondEvent;

	EventManager_unregisterEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_1 );

	EventManager_RegisteredEvent* firstRegisteredEvent = eventManager->events->data;
	TestFramework_EXPECT( eventManager->events != NULL, "events pointer not to be NULL after removing RegisteredEvent with others remaining" );
	TestFramework_EXPECT( firstRegisteredEvent->eventKey == EventManagerTests_DEMO_EVENT_2, "first RegisteredEvent key to now be EventManagerTests_DEMO_EVENT_2" );
	TestFramework_EXPECT( eventManager->events->next == NULL, "first RegisteredEvent container node not to be pointing to anything more" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

TestFramework_TestResult EventManagerTests_verifyUnregisterEntireSecondEventWithOthers() {
	TestFramework_TestResult testResult;
	EventManager* eventManager;
	EventManagerTests_ExampleObject* exampleInstance = calloc( 1, sizeof( EventManagerTests_ExampleObject ) );

	eventManager = calloc( 1, sizeof( EventManager ) );
	EventManager_ctor( eventManager );

	EventManagerTests_setupMockEvents( eventManager, exampleInstance );

	// Add one for DEMO_EVENT_2
	EventManager_RegisteredEvent* secondEvent = calloc( 1, sizeof( EventManager_RegisteredEvent ) );
	secondEvent->eventKey = EventManagerTests_DEMO_EVENT_2;
	secondEvent->listeners = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( secondEvent->listeners );

	EventManager_EventListener* eventListener2 = calloc( 1, sizeof( EventManager_EventListener ) );
	eventListener2->instance = exampleInstance;
	eventListener2->callback = EventManagerTests_eventTarget;

	secondEvent->listeners->data = eventListener2;

	eventManager->events->next = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( eventManager->events->next );
	eventManager->events->next->data = secondEvent;

	EventManager_unregisterEvent( eventManager, exampleInstance, EventManagerTests_DEMO_EVENT_2 );

	EventManager_RegisteredEvent* firstRegisteredEvent = eventManager->events->data;
	TestFramework_EXPECT( eventManager->events != NULL, "events pointer not to be NULL after removing RegisteredEvent with others remaining" );
	TestFramework_EXPECT( firstRegisteredEvent->eventKey == EventManagerTests_DEMO_EVENT_1, "first RegisteredEvent key to now be EventManagerTests_DEMO_EVENT_1" );
	TestFramework_EXPECT( eventManager->events->next == NULL, "first RegisteredEvent container node not to be pointing to anything more" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	EventManager_dtor( eventManager );
	free( exampleInstance );

	return testResult;
}

void EventManagerTests_setupMockEvents( EventManager* eventManager, void* instance ) {
	// One RegisteredEvent
	EventManager_RegisteredEvent* event = calloc( 1, sizeof( EventManager_RegisteredEvent ) );
	event->eventKey = EventManagerTests_DEMO_EVENT_1;
	event->listeners = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( event->listeners );

	// One listener for this instance
	EventManager_EventListener* eventListener = calloc( 1, sizeof( EventManager_EventListener ) );
	eventListener->instance = instance;
	eventListener->callback = EventManagerTests_eventTarget;

	// add EventListener to RegisteredEvent above
	event->listeners->data = eventListener;

	// Setup events linked list on eventManager
	eventManager->events = calloc( 1, sizeof( LinkedListNode ) );
	LinkedListNode_ctor( eventManager->events );

	// Add the RegisteredEvent to the linked list
	eventManager->events->data = event;
}

void EventManagerTests_eventTarget( void* context, void* payload ) {
	EventManagerTests_ExampleObject* this = ( EventManagerTests_ExampleObject* ) context;

	this->one = 0x01;
	if( payload != NULL ) {
		this->two = 0x02;
	}
}

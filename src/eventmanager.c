#include <eventmanager.h>
#include <romble.h>
#include <types.h>
#include <linkedlist.h>
#include <res/globals.h>

// bit of a crappy way to generate a function for a function predicate
static EventManager_EventKey EventManager_isEvent_EVENT_KEY = EventManager_EVENT_NULL;
static void* EventManager_eventExists_INSTANCE_KEY = NULL;

void EventManager_ctor( EventManager* this ) {
	this->events = NULL;
}

void EventManager_dtor( EventManager* this ) {
	// Iterate through each EventManager_RegisteredEvent, freeing the RAM occupied by each and the EventManager_EventListener within
}

/**
 * Let the LinkedListNode class handle the adding!
 */
void EventManager_registerEvent( EventManager* this, void* hostInstance, EventManager_EventKey eventKey, EventManager_Callback callback ) {
	// Has the event already been added to the linked list chain in this->events?
	EventManager_isEvent_EVENT_KEY = eventKey;
	EventManager_RegisteredEvent* registeredEvent = LinkedListNode_findData( this->events, EventManager_isEvent );
	if( registeredEvent != NULL ) {
		// This event already exists - Add a new EventListener to it, provided this instance already isn't already listening!
		EventManager_eventExists_INSTANCE_KEY = hostInstance;
		EventManager_EventListener* eventListener = LinkedListNode_findData( registeredEvent->listeners, EventManager_eventExists );
		if( eventListener == NULL ) {
			eventListener = Romble_alloc_d( sizeof( EventManager_EventListener ), TRUE, FILE_LINE() );
			LinkedListNode_insertEnd( registeredEvent->listeners, eventListener );
		}
	} else {
		// This event does not have any listeners attached to it - The event object needs to be added,
		// and then the initial event listener needs to be added

		// But wait! Does the first node need to be created?
		if( this->events == NULL ) {
			// Create the first node
			this->events = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
			LinkedListNode_ctor( this->events );
		}

		// Craete the event object
		registeredEvent = Romble_alloc_d( sizeof( EventManager_RegisteredEvent ), TRUE, FILE_LINE() );
		registeredEvent->eventKey = eventKey;
		registeredEvent->listeners = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
		LinkedListNode_ctor( registeredEvent->listeners );

		// Create the event listener object
		EventManager_EventListener* eventListener = Romble_alloc_d( sizeof( EventManager_EventListener ), TRUE, FILE_LINE() );
		eventListener->instance = hostInstance;
		eventListener->callback = callback;

		// Add the event listener to the registered event object
		LinkedListNode_insertEnd( registeredEvent->listeners, eventListener );

		// Add the event object to this->events
		LinkedListNode_insertEnd( this->events, registeredEvent );
	}
}

bool EventManager_isEvent( void* registeredEvent ) {
	if( registeredEvent == NULL ) {
		return FALSE;
	}

	EventManager_RegisteredEvent* event = ( EventManager_RegisteredEvent* ) registeredEvent;
	return event->eventKey == EventManager_isEvent_EVENT_KEY;
}

bool EventManager_eventExists( void* eventListener ) {
	if( eventListener == NULL ) {
		return FALSE;
	}

	EventManager_EventListener* listener = ( EventManager_EventListener* ) eventListener;
	return listener->instance == EventManager_eventExists_INSTANCE_KEY;
}

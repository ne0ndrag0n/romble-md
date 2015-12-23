#include <eventmanager.h>
#include <romble.h>
#include <types.h>
#include <linkedlist.h>
#include <res/globals.h>
#include <utility.h>

// bit of a crappy way to generate a function for a function predicate
static EventManager_EventKey EventManager_isEvent_EVENT_KEY = EventManager_EVENT_NULL;
static void* EventManager_eventExists_INSTANCE_KEY = NULL;

void EventManager_ctor( EventManager* this ) {
	this->events = NULL;
}

void EventManager_dtor( EventManager* this ) {
	// Iterate through each EventManager_RegisteredEvent, freeing the RAM occupied by each and the EventManager_EventListener within
	LinkedListNode* currentEventNode = this->events;

	while( currentEventNode != NULL ) {
		LinkedListNode* currentListenerNode = ( ( EventManager_RegisteredEvent* )( currentEventNode->data ) )->listeners;

		// Free all the data within each linked list node for this registered event's listeners
		while( currentListenerNode != NULL ) {
			Romble_free_d( currentListenerNode->data, FILE_LINE() );

			currentListenerNode = currentListenerNode->next;
		}

		// Free the entire listeners chain
		LinkedListNode_dtor( ( ( EventManager_RegisteredEvent* )( currentEventNode->data ) )->listeners );

		// Free the actual event data
		Romble_free_d( currentEventNode->data, FILE_LINE() );

		currentEventNode = currentEventNode->next;
	}

	// With all components within this->events freed, call destructor on the top-level linked list
	LinkedListNode_dtor( this->events );
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
			eventListener->instance = hostInstance;
			eventListener->callback = callback;

			LinkedListNode_insertEnd( registeredEvent->listeners, eventListener );
		}
	} else {
		// This event does not have any listeners attached to it - The event object needs to be created, added,
		// and then the initial event listener needs to be added

		// Create the event object
		registeredEvent = Romble_alloc_d( sizeof( EventManager_RegisteredEvent ), TRUE, FILE_LINE() );
		registeredEvent->eventKey = eventKey;
		registeredEvent->listeners = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
		LinkedListNode_ctor( registeredEvent->listeners );

		// Create the event listener object
		EventManager_EventListener* eventListener = Romble_alloc_d( sizeof( EventManager_EventListener ), TRUE, FILE_LINE() );
		eventListener->instance = hostInstance;
		eventListener->callback = callback;

		// Add the event listener to the registered event object; it is the first node, so insertEnd will not work properly!
		registeredEvent->listeners->data = eventListener;

		// Add the event object to this->events. Check if this->events was created previously; if it wasn't, create this->events
		// and set the pointer (insertEnd does not work in that instance!). If this->events exists, *THEN* use insertEnd.
		// But wait! Does the first node need to be created?
		if( this->events == NULL ) {
			// Create the first node
			this->events = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
			LinkedListNode_ctor( this->events );

			this->events->data = registeredEvent;
		} else {
			LinkedListNode_insertEnd( this->events, registeredEvent );
		}
	}
}

void EventManager_unregisterEvent( EventManager* this, void* hostInstance, EventManager_EventKey eventKey ) {

	if( this->events == NULL ) {
		return;
	}

	// Get the RegisteredEvent object
	EventManager_isEvent_EVENT_KEY = eventKey;
	EventManager_RegisteredEvent* registeredEvent = LinkedListNode_findData( this->events, EventManager_isEvent );

	if( registeredEvent != NULL ) {
		// Set the function predicate to look for the EventListener with hostInstance attached
		EventManager_eventExists_INSTANCE_KEY = hostInstance;
		// Get reference for when we need to free it based on the instance (will return NULL if not found as expected)
		EventManager_EventListener* eventListener = LinkedListNode_findData( registeredEvent->listeners, EventManager_eventExists );
		// Remove the event listener from the linked list of listeners, where hostInstance is the instance (will do nothing if not found, as expected)
		LinkedListNode_remove( &( registeredEvent->listeners ), EventManager_eventExists, FALSE );
		// Free the actual eventListener (removing the linked list node just leaves it in place)
		Romble_free_d( eventListener, FILE_LINE() );

		// If this was the last EventListener, clean up the RegisteredEvent
		// LinkedListNode will set registeredEvent->listeners to a NULL pointer
		if( registeredEvent->listeners == NULL ) {
			EventManager_isEvent_EVENT_KEY = eventKey;
			LinkedListNode_remove( &( this->events ), EventManager_isEvent, FALSE );

			Romble_free_d( registeredEvent, FILE_LINE() );
		}
	}

}

void EventManager_trigger( EventManager* this, EventManager_EventKey eventKey, void* payload ) {
	// Get RegisteredEvent by eventKey
	EventManager_isEvent_EVENT_KEY = eventKey;
	EventManager_RegisteredEvent* registeredEvent = LinkedListNode_findData( this->events, EventManager_isEvent );

	if( registeredEvent != NULL ) {
		// For each EventListener on this RegisteredEvent, synchronously call the callback with the
		// attached instance and given payload.
		LinkedListNode* current = registeredEvent->listeners;
		while( current != NULL ) {
			EventManager_EventListener* eventListener = ( EventManager_EventListener* ) current->data;
			if( eventListener->callback != NULL ) {
				eventListener->callback( eventListener->instance, payload );
			}

			current = current->next;
		}
	}
}

/**
 * Finds a RegisteredEvent by event key
 */
bool EventManager_isEvent( void* registeredEvent ) {
	if( registeredEvent == NULL ) {
		return FALSE;
	}

	EventManager_RegisteredEvent* event = ( EventManager_RegisteredEvent* ) registeredEvent;
	return event->eventKey == EventManager_isEvent_EVENT_KEY;
}

/**
 * Finds an EventListener by instance pointer
 */
bool EventManager_eventExists( void* eventListener ) {
	if( eventListener == NULL ) {
		return FALSE;
	}

	EventManager_EventListener* listener = ( EventManager_EventListener* ) eventListener;
	return listener->instance == EventManager_eventExists_INSTANCE_KEY;
}

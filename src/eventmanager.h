/**
 * Synchronous event management subsystem. Triggers blocking events
 * that other objects may attach and listen for.
 *
 * @author		Ash
 * @version		0.0.1
 */
#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include <types.h>
#include <res/globals.h>
#include <linkedlist.h>

#define		EventManager_EVENT_NULL		0x0000

struct EventManager;

typedef u16 EventManager_EventKey;
// instance, payload
typedef void ( *EventManager_Callback )( void*, void* );

typedef struct EventManager_RegisteredEvent {
	EventManager_EventKey eventKey;
	LinkedListNode* listeners;
} EventManager_RegisteredEvent;

typedef struct EventManager_EventListener {
	void* instance;
	EventManager_Callback callback;
} EventManager_EventListener;

typedef struct EventManager {
	LinkedListNode* events;
} EventManager;

void EventManager_ctor( EventManager* this );
void EventManager_dtor( EventManager* this );

void EventManager_registerEvent( EventManager* this, void* hostInstance, EventManager_EventKey eventKey, EventManager_Callback callback );
void EventManager_unregisterEvent( EventManager* this, void* hostInstance, EventManager_EventKey eventKey );

void EventManager_trigger( EventManager* this, EventManager_EventKey eventKey, void* payload );

bool EventManager_isEvent( void* registeredEvent );
bool EventManager_eventExists( void* eventListener );

#endif

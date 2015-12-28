#ifndef SUPERVISOR
#define SUPERVISOR

#include <types.h>
#include <res/globals.h>
#include <baseview.h>

struct Supervisor;

typedef enum {
	Supervisor_State_DEFAULT = 0,
	Supervisor_State_WELCOME,
	Supervisor_State_LOGIN,
	Supervisor_State_BETATEST
} Supervisor_State;

typedef struct {
	void ( *destroy )( struct BaseView* );
	void ( *render )( struct Supervisor* );
	void ( *position )( struct BaseView* );
	void ( *renderChildren )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *placeTile )( struct BaseView*, s16, s16, s16, u8, u16, bool, bool );
	void ( *placeTileSeries )( struct BaseView*, s16, s16, s16, s16, s16, u8, u16, bool );

	void ( *listenToView )( struct BaseView*, struct BaseView*, EventManager_EventKey, EventManager_Callback );
	void ( *stopListeningView )( struct BaseView*, struct BaseView*, EventManager_EventKey );
	void ( *trigger )( struct BaseView*, EventManager_EventKey, void* );

	struct BaseView* ( *getChildById )( struct BaseView*, u16 );
	struct BaseView* ( *getChildByTag )( struct BaseView*, u16 );
	void ( *removeChild )( struct BaseView* , struct BaseView* );

	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );

	void ( *changeState )( struct Supervisor*, Supervisor_State );
	void ( *clearPlane )( struct Supervisor* );
} Supervisor_vtable;

typedef struct Supervisor {
	BaseView super;

	BaseView* stateView;
	Supervisor_State currentState;
} Supervisor;

void Supervisor_ctor( Supervisor* this );

void Supervisor_render( Supervisor* this );

void Supervisor_clearPlane( Supervisor* this );
/**
 * This function will be called on receipt of the EVENT_NAVIGATE event on the global EventManager. It may
 * also be called directly (main.c). This function's responsibility is to remove
 */
void Supervisor_changeState( Supervisor* this, Supervisor_State state );
void Supervisor_onChangeState( void* instance, void* payload );

#endif

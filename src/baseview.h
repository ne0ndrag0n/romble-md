#ifndef BASEVIEW
#define BASEVIEW

#include <types.h>
#include <vdp.h>
#include <res/globals.h>
#include <linkedlist.h>
#include <eventmanager.h>

struct BaseView;

typedef struct {
	void ( *destroy )( struct BaseView* );
	void ( *render )( struct BaseView* );
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
	void ( *removeChild )( struct BaseView* , struct BaseView*, bool );

	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );
} BaseView_vtable;

typedef struct BaseView {
	u16 id;
	u16 tag;

	s16 x;
	s16 y;

	s16 width;
	s16 height;

	s16 absX;
	s16 absY;

	u8 numChildren;

	struct BaseView* parent;
	LinkedListNode* children;
	EventManager* events;

	vtable functions;
} BaseView;

void BaseView_ctor( BaseView* this, s16 x, s16 y, s16 width, s16 height );
void BaseView_dtor( BaseView* this );

u8 BaseView_testa( BaseView* this );

void BaseView_render( BaseView* this );
void BaseView_position( BaseView* this );
void BaseView_renderChildren( BaseView* this );
void BaseView_addChildView( BaseView* this, BaseView* childView );

void BaseView_placeTile( BaseView* this, s16 plane, s16 x, s16 y, u8 pal, u16 tileIndex, bool flipV, bool flipH );
void BaseView_placeTileSeries( BaseView* this, s16 plane, s16 x, s16 y, s16 w, s16 h, u8 pal, u16 tileIndex, bool autoInc );

/**
 * Target will have an event registered in its local EventManager, with this as the callback context. Your callbacks should be located
 * in "this", the originator of the listenTo call.
 */
void BaseView_listenToView( BaseView* this, BaseView* target, EventManager_EventKey event, EventManager_Callback callback );
void BaseView_stopListeningView( BaseView* this, BaseView* target, EventManager_EventKey event );
/**
 * Trigger fires off all events of type "event" in this's eventManager
 */
void BaseView_trigger( BaseView* this, EventManager_EventKey event, void* payload );

BaseView* BaseView_getChildById( BaseView* this, u16 id );
BaseView* BaseView_getChildByTag( BaseView* this, u16 tag );

void BaseView_removeChild( BaseView* this, BaseView* childInstance, bool rerender );

bool BaseView_checkTileBoundary( BaseView* this, s16 x, s16 y );
bool BaseView_isView( void* view );
bool BaseView_isViewByTag( void* instance );
bool BaseView_isInstance( void* instance );
void BaseView_freeInstance( void* instance );

#endif

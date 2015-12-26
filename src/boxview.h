#ifndef BOXVIEW
#define BOXVIEW

#include <types.h>
#include <baseview.h>
#include <res/globals.h>
#include <eventmanager.h>

struct BoxView;

typedef struct {
	void ( *destroy )( struct BaseView* );
	void ( *render )( struct BoxView* );
	void ( *position )( struct BaseView* );
	void ( *renderChildren )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *placeTile )( struct BaseView*, s16, s16, s16, u8, u16, bool, bool );
	void ( *placeTileSeries )( struct BaseView*, s16, s16, s16, s16, s16, u8, u16, bool );

	void ( *listenToView )( struct BaseView*, struct BaseView*, EventManager_EventKey, EventManager_Callback );
	void ( *stopListeningView )( struct BaseView*, struct BaseView*, EventManager_EventKey );
	void ( *trigger )( struct BaseView*, EventManager_EventKey, void* );

	struct BaseView* ( *getChildById )( struct BaseView*, u16 );

	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );
} BoxView_vtable;

typedef struct BoxView {
	BaseView super;

	u16 fillTileIndex;
	u16 boxDrawingIndex;
} BoxView;

void BoxView_ctor( BoxView* this, u16 boxDrawingIndex, u16 fillTileIndex, s16 x, s16 y, s16 width, s16 height );
void BoxView_render( BoxView* this );


#endif

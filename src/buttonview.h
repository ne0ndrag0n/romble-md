#ifndef BUTTONVIEW
#define BUTTONVIEW

#include <baseview.h>
#include <boxview.h>
#include <simpletextview.h>
#include <types.h>
#include <res/globals.h>
#include <eventmanager.h>

struct ButtonView;

typedef struct {
	void ( *destroy )( struct ButtonView* );
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
	struct BaseView* ( *getChildByTag )( struct BaseView*, u16 );
	void ( *removeChild )( struct BaseView* , struct BaseView*, bool );

	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );

	void ( *setText )( struct ButtonView*, char*, bool, bool );
	void ( *setClickable )( struct ButtonView*, bool );
	void ( *onClick )( void*, u16 );
} ButtonView_vtable;

typedef enum {
	ButtonView_TextPosition_LEFT,
	ButtonView_TextPosition_CENTER
} ButtonView_TextPosition;

typedef struct ButtonView {
	BoxView super;

	SimpleTextView* label;
} ButtonView;

void ButtonView_ctor( ButtonView* this, u16 boxDrawingIndex, u16 fillTileIndex, s16 x, s16 y, s16 width );
void ButtonView_dtor( ButtonView* this );

void ButtonView_setText( ButtonView* this, char* label, bool render, bool copy );
void ButtonView_setClickable( ButtonView* this, bool clickable );

void ButtonView_onClick( void* instance, u16 button );

#endif

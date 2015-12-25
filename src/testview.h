#ifndef TESTVIEW
#define TESTVIEW

#include <types.h>
#include <baseview.h>
#include <boxview.h>
#include <simpletextview.h>
#include <joymanager.h>
#include <buttonview.h>
#include <vdpmanager.h>
#include <eventmanager.h>

struct TestView;

typedef struct {
	void ( *destroy )( struct TestView* );
	void ( *render )( struct TestView* );
	void ( *position )( struct BaseView* );
	void ( *renderChildren )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, s16, s16, s16, u8, u16, bool, bool );
	void ( *placeTileSeries )( struct BaseView*, s16, s16, s16, s16, s16, u8, u16, bool );

	void ( *listenToView )( struct BaseView*, struct BaseView*, EventManager_EventKey, EventManager_Callback );
	void ( *stopListeningView )( struct BaseView*, struct BaseView*, EventManager_EventKey );
	void ( *trigger )( struct BaseView*, EventManager_EventKey, void* );

	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );

	void ( *setupChildren )( struct TestView* );
} TestView_vtable;

typedef struct TestView {
	BaseView super;

	ButtonView* hi;
	ButtonView* bye;
	ButtonView* obscenity;

	SimpleTextView* textView;
	ButtonView* displayedText;
	ButtonView* clear;

	VDPManager_TileIndex buttonStyle;
} TestView;

void TestView_ctor( TestView* this, s16 x, s16 y, s16 width, s16 height );
void TestView_dtor( TestView* this );

void TestView_render( TestView* this );

void TestView_setupChildren( TestView* this );

void TestView_onButtonClick( void* instance, void* payload );

#endif

#ifndef TESTVIEW
#define TESTVIEW

#include <types.h>
#include <baseview.h>
#include <boxview.h>
#include <simpletextview.h>

struct TestView;

typedef struct {
	void ( *TestView )( struct TestView*, s16, s16, s16, s16 );
	void ( *destroy )( struct TestView* );
	void ( *render )( struct BaseView* );
	void ( *position )( struct BaseView* );
	void ( *renderChildren )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, s16, s16, u8, u16, bool, bool );
	void ( *placeTileSeries )( struct BaseView*, s16, s16, s16, s16, u8, u16, bool );
	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );
} TestView_vtable;

typedef struct TestView {
	BaseView super;
	
	BoxView* hiBox;
	SimpleTextView* hiText;
} TestView;

void TestView_ctor( TestView* this, s16 x, s16 y, s16 width, s16 height );
void TestView_dtor( TestView* this );

#endif

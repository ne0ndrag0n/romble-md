#ifndef SIMPLETEXTVIEW
#define SIMPLETEXTVIEW

#include <types.h>
#include <baseview.h>
#include <res/globals.h>

struct SimpleTextView;

typedef struct {
	void ( *destroy )( struct SimpleTextView* );
	void ( *render )( struct SimpleTextView* );
	void ( *position )( struct BaseView* );
	void ( *renderChildren )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, s16, s16, u8, u16, bool, bool );
	void ( *placeTileSeries )( struct BaseView*, s16, s16, s16, s16, u8, u16, bool );
	bool ( *checkTileBoundary )( struct BaseView*, s16, s16 );

	void( *setText )( struct SimpleTextView*, char* );
} SimpleTextView_vtable;

typedef struct SimpleTextView {
	BaseView super;

	char* text;
} SimpleTextView;

void SimpleTextView_ctor( SimpleTextView* this, char* text, s16 x, s16 y );
void SimpleTextView_dtor( SimpleTextView* this );
void SimpleTextView_render( SimpleTextView* this );

void SimpleTextView_setText( SimpleTextView* this, char* text );

#endif

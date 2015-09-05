#ifndef SIMPLETEXTVIEW
#define SIMPLETEXTVIEW

#include <types.h>
#include <baseview.h>
#include <res/globals.h>

struct SimpleTextView;

typedef struct {
	void ( *SimpleTextView )( struct SimpleTextView*, char* text, u8, u8 );
	u8 ( *testa )( struct BaseView* );
	void ( *render )( struct SimpleTextView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, u8, u8, u8, u16, bool, bool );
} SimpleTextView_vtable;

typedef struct SimpleTextView {
	BaseView super;

	char* text;
} SimpleTextView;

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y );
void SimpleTextView_render( SimpleTextView* this );

#endif

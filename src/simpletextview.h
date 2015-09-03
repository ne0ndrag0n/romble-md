#ifndef SIMPLETEXTVIEW
#define SIMPLETEXTVIEW

#include <types.h>
#include <baseview.h>

struct SimpleTextView;

typedef struct {
	void ( *SimpleTextView )( struct SimpleTextView*, char* text, u8, u8 );
	u8 ( *testa )( struct SimpleTextView* );
	void ( *render )( struct SimpleTextView* );
} SimpleTextView_vtable;

typedef struct SimpleTextView {
	BaseView super;

	char* text;
} SimpleTextView;

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y );
void SimpleTextView_render( SimpleTextView* this );
u8 SimpleTextView_testa( SimpleTextView* this );

#endif

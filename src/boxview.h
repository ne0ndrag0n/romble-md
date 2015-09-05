#ifndef BOXVIEW
#define BOXVIEW

#include <types.h>
#include <baseview.h>
#include <res/globals.h>

struct BoxView;

typedef struct {
	void ( *BoxView )( struct BoxView*, u16, u8, u8, u8, u8 );
	void ( *destroy )( struct BaseView* );
	void ( *render )( struct BoxView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, u8, u8, u8, u16, bool, bool );
} BoxView_vtable;

typedef struct BoxView {
	BaseView super;

	// In format: upper right, width, upper left, length
	u16 boxDrawingIndex;
} BoxView;

void BoxView_ctor( BoxView* this, u16 boxDrawingIndex, u8 x, u8 y, u8 width, u8 height );
void BoxView_render( BoxView* this );


#endif

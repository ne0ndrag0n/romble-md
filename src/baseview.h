#ifndef BASEVIEW
#define BASEVIEW

#include <types.h>
#include <vdp.h>
#include <res/globals.h>

struct BaseView;

typedef struct {
	void ( *BaseView )( struct BaseView*, u8, u8, u8, u8 );
	void ( *destroy )( struct BaseView* );
	void ( *render )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
	void ( *setPlane )( struct BaseView*, u16 );
	void ( *placeTile )( struct BaseView*, u8, u8, u8, u16, bool, bool );
} BaseView_vtable;

typedef struct BaseView {
	u8 x;
	u8 y;

	u8 width;
	u8 height;

	u8 absX;
	u8 absY;

	u8 numChildren;
	u16 plane;

	struct BaseView* parent;
	struct BaseView** children;

	BaseView_vtable *functions;
} BaseView;

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height );
void BaseView_dtor( BaseView* this );

u8 BaseView_testa( BaseView* this );

void BaseView_render( BaseView* this );
void BaseView_addChildView( BaseView* this, BaseView* childView );

void BaseView_setPlane( BaseView* this, u16 plane );
void BaseView_placeTile( BaseView* this, u8 x, u8 y, u8 pal, u16 tileIndex, bool flipV, bool flipH );

#endif

#ifndef BASEVIEW
#define BASEVIEW

#include <types.h>
#include <res/globals.h>

struct BaseView;

typedef struct {
	void ( *BaseView )( struct BaseView*, u8, u8, u8, u8 );
	u8 ( *testa )( struct BaseView* );
	void ( *render )( struct BaseView* );
	void ( *addChildView )( struct BaseView*, struct BaseView* );
} BaseView_vtable;

typedef struct BaseView {
	u8 x;
	u8 y;

	u8 width;
	u8 height;

	u8 absX;
	u8 absY;

	struct BaseView* parent;
	struct BaseView** children;
	u8 numChildren;

	BaseView_vtable *functions;
} BaseView;

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height );
u8 BaseView_testa( BaseView* this );

void BaseView_render( BaseView* this );
void BaseView_addChildView( BaseView* this, BaseView* childView );

#endif

#ifndef BASEVIEW
#define BASEVIEW

#include <types.h>

struct BaseView;

typedef struct {
	void ( *BaseView )( struct BaseView*, u8, u8, u8, u8 );
	u8 ( *testa )( struct BaseView* );
	void ( *render )( struct BaseView* );
} BaseView_vtable;

typedef struct BaseView {
	u8 x;
	u8 y;

	u8 width;
	u8 height;

	u8 absX;
	u8 absY;

	struct BaseView* parent;
	BaseView_vtable *functions;
} BaseView;

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height );
u8 BaseView_testa( BaseView* this );

void BaseView_render( BaseView* this );

#endif

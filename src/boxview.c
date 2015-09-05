#include <boxview.h>
#include <baseview.h>
#include <res/globals.h>
#include <stddef.h>

BoxView_vtable BoxView_table = { 
	BoxView_ctor, 
	BaseView_testa, 
	BoxView_render,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile
};

void BoxView_ctor( BoxView* this, u16 boxDrawingIndex, u8 x, u8 y, u8 width, u8 height ) {
	BaseView_ctor( ( BaseView* ) this, x, y, width, height );
	
	this->boxDrawingIndex = boxDrawingIndex;

	this->super.functions = &BoxView_table;
}

void BoxView_render( BoxView* this ) {
	BaseView_render( ( BaseView* ) this );

	void ( *placeTile )( struct BaseView*, u8, u8, u8, u16, bool, bool ) = this->super.functions->placeTile;

	// Place corner tiles
	placeTile( ( BaseView* ) this, 0, 0, PAL0, this->boxDrawingIndex, FALSE, FALSE );
	placeTile( ( BaseView* ) this, ( ( BaseView* )this )->width - 1, 0, PAL0, this->boxDrawingIndex, FALSE, TRUE );
	placeTile( ( BaseView* ) this, ( ( BaseView* )this )->width - 1, ( ( BaseView* ) this )->height - 1, PAL0, this->boxDrawingIndex, TRUE, TRUE );
	placeTile( ( BaseView* ) this, 0, ( ( BaseView* ) this )->height - 1, PAL0, this->boxDrawingIndex, TRUE, FALSE );

	// Place tile runs
	size_t i;
	for( i = 1; i != ( ( BaseView* ) this )->width - 1; i++ ) {
		// Horizontal runs
		placeTile( ( BaseView* ) this, i, 0, PAL0, this->boxDrawingIndex + 1, FALSE, FALSE );
		placeTile( ( BaseView* ) this, i, ( ( BaseView* ) this )->height - 1, PAL0, this->boxDrawingIndex + 1, FALSE, FALSE );
	}

	for( i = 1; i != ( ( BaseView* ) this )->height - 1; i++ ) {
		// Vertical runs
		placeTile( ( BaseView* ) this, 0, i, PAL0, this->boxDrawingIndex + 2, FALSE, FALSE );
		placeTile( ( BaseView* ) this, ( ( BaseView* ) this )->width - 1, i, PAL0, this->boxDrawingIndex + 2, FALSE, FALSE );
	}
}

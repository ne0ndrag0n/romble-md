#include <boxview.h>
#include <baseview.h>
#include <res/globals.h>
#include <stddef.h>

BoxView_vtable BoxView_table = { 
	BoxView_ctor, 
	BaseView_dtor, 
	BoxView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary
};

void BoxView_ctor( BoxView* this, u16 boxDrawingIndex, u16 fillTileIndex, u8 x, u8 y, u8 width, u8 height ) {
	BaseView_ctor( ( BaseView* ) this, x, y, width, height );
	
	this->fillTileIndex = fillTileIndex;
	this->boxDrawingIndex = boxDrawingIndex;

	this->super.functions = &BoxView_table;
}

void BoxView_render( BoxView* this ) {
	BaseView* super = ( BaseView* ) this;

	BaseView_render( super );

	void ( *placeTile )( struct BaseView*, u8, u8, u8, u16, bool, bool ) = this->super.functions->placeTile;

	// Place corner tiles
	placeTile( super, 0, 0, PAL0, this->boxDrawingIndex, FALSE, FALSE );
	placeTile( super, super->width - 1, 0, PAL0, this->boxDrawingIndex, FALSE, TRUE );
	placeTile( super, super->width - 1, super->height - 1, PAL0, this->boxDrawingIndex, TRUE, TRUE );
	placeTile( super, 0, super->height - 1, PAL0, this->boxDrawingIndex, TRUE, FALSE );

	// Place tile runs
	size_t i, j;
	for( i = 1; i != super->width - 1; i++ ) {
		// Horizontal runs
		placeTile( super, i, 0, PAL0, this->boxDrawingIndex + 1, FALSE, FALSE );
		placeTile( super, i, super->height - 1, PAL0, this->boxDrawingIndex + 1, TRUE, FALSE );
	}

	for( i = 1; i != super->height - 1; i++ ) {
		// Vertical runs
		placeTile( super, 0, i, PAL0, this->boxDrawingIndex + 2, FALSE, TRUE );
		placeTile( super, super->width - 1, i, PAL0, this->boxDrawingIndex + 2, FALSE, FALSE );
	}

	// Make the inside solid
	/*
	for ( i = 1; i != super->height - 1; i++ ) {
		for( j = 1; j != super->width - 1; j++ ) {
			placeTile( super, j, i, PAL0, this->fillTileIndex, FALSE, FALSE );
		}
	}*/
}

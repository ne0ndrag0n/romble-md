/**
 * oaktree MdVC BaseView
 * (c) 2015 Oaktree Games and Novelties
 */
#include <baseview.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys.h>
#include <lang.h>
#include <res/globals.h>
#include <utility.h>
#include <vdp.h>
#include <vdp_tile.h>
#include <stdio.h>

BaseView_vtable BaseView_table = { 
	BaseView_ctor, 
	BaseView_testa, 
	BaseView_render,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile
};

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height ) {
	this->functions = &BaseView_table;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;

	this->children = NULL;
	this->parent = NULL;
	this->numChildren = 0;
	this->plane = VDP_PLAN_A;
}

u8 BaseView_testa( BaseView* this ) {
	return this->width + this->height;
}

void BaseView_render( BaseView* this ) {
	// If this view has no parent, set absX and absY to where the view is placed....else, add the absX and absY of the parent
	if( this->parent == NULL ) {
		this->absX = this->x;
		this->absY = this->y;
	} else {
		this->absX = this->x + this->parent->absX;
		this->absY = this->y + this->parent->absY;
	}
	
	// Place and render children
	if( this->children != NULL ) {
		size_t i;

		for( i = 0; i != this->numChildren; i++ ) {
			BaseView* view = this->children[ i ];

			view->functions->render( view );
		}
	}
}

void BaseView_addChildView( BaseView* this, BaseView* childView ) {
	// Create the children collection if it does not exist
	if( this->children == NULL ) {
		this->children = ( BaseView** ) malloc( sizeof( BaseView* ) );
		
		if( this->children == NULL ) {
			SYS_die( EXCEPTION_OUT_OF_MEMORY );
		}

		this->numChildren++;
	} else {

		this->numChildren++;
		BaseView** resizedArray = ( BaseView** ) realloc( this->children, this->numChildren );
		if( resizedArray == NULL ) {
			SYS_die( EXCEPTION_OUT_OF_MEMORY );
		} else {
			this->children = resizedArray;
		}
	}
	
	// After resized, set the last elemnt in the array to childView
	childView->parent = this;
	this->children[ this->numChildren - 1 ] = childView;
}

void BaseView_setPlane( BaseView* this, u16 plane ) {
	this->plane = plane;
}

void BaseView_placeTile( BaseView* this, u8 x, u8 y, u8 pal, u16 tileIndex, bool flipV, bool flipH ) {
	// If x or y lie outside the boundaries, the tile will not be visible. Do not draw it.
	u8 absX = x + this->absX;
	u8 absY = y + this->absY;

	if( ( this->parent == NULL ) || ( absX < ( this->parent->absX + this->parent->width ) && absY < ( this->parent->absY + this->parent->height ) ) ) {
		VDP_setTileMapXY( this->plane, TILE_ATTR_FULL( pal, PRIORITY_LOW, flipV, flipH, tileIndex ), absX, absY );
	}
}

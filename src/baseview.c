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
#include <joy.h>
#include <romble.h>
#include <timer.h>
#include <joymanager.h>
#include <linkedlist.h>
#include <eventmanager.h>

static u16 BaseView_isView_ID = 0;

BaseView_vtable BaseView_table = {
	BaseView_dtor,
	BaseView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary
};

void BaseView_ctor( BaseView* this, s16 x, s16 y, s16 width, s16 height ) {
	this->id = Romble_getUniqueId();

	this->functions = &BaseView_table;

	this->absX = 0;
	this->absY = 0;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;

	this->scrollX = 0;
	this->scrollY = 0;

	this->numChildren = 0;
	this->plane = VDP_PLAN_A;

	this->children = NULL;
	this->parent = NULL;
	this->events = Romble_alloc_d( sizeof( EventManager ), TRUE, FILE_LINE() );

	EventManager_ctor( this->events );
}

void BaseView_dtor( BaseView* this ) {
	EventManager_dtor( this->events );

	LinkedListNode_dtor( this->children );
}

void BaseView_render( BaseView* this ) {
	FUNCTIONS( BaseView, BaseView, this )->position( this );

	FUNCTIONS( BaseView, BaseView, this )->renderChildren( this );
}

void BaseView_position( BaseView* this ) {
	// If this view has no parent, set absX and absY to where the view is placed....else, add the absX and absY of the parent
	if( this->parent == NULL ) {
		this->absX = this->x;
		this->absY = this->y;
	} else {
		this->absX = this->x + this->parent->absX + this->parent->scrollX;
		this->absY = this->y + this->parent->absY + this->parent->scrollY;
	}
}

void BaseView_renderChildren( BaseView* this ) {
	// Place and render children
	LinkedListNode* current = this->children;
	while( current != NULL ) {
		BaseView* view = current->data;

		FUNCTIONS( BaseView, BaseView, view )->render( view );

		current = current->next;
	}
}

void BaseView_addChildView( BaseView* this, BaseView* childView ) {
	// Create the children collection if it does not exist
	// Populate the first node
	if( this->children == NULL ) {
		this->children = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
		Romble_assert( this->children != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
		LinkedListNode_ctor( this->children );
		this->children->data = childView;
	} else {
		// Toss this at the end of the list if there's already a node
		LinkedListNode_insertEnd( this->children, childView );
	}

	childView->parent = this;
	this->numChildren++;
}

void BaseView_setPlane( BaseView* this, u16 plane ) {
	this->plane = plane;
}

void BaseView_placeTile( BaseView* this, s16 x, s16 y, u8 pal, u16 tileIndex, bool flipV, bool flipH ) {
	// If x or y lie outside the boundaries, the tile will not be visible. Do not draw it.
	s16 absX = x + this->absX;
	s16 absY = y + this->absY;

	// Check that the tile can be placed within this container as well as all parent containers
	if( FUNCTIONS( BaseView, BaseView, this )->checkTileBoundary( this, absX, absY ) ) {
		VDP_setTileMapXY( this->plane, TILE_ATTR_FULL( pal, PRIORITY_LOW, flipV, flipH, tileIndex ), absX, absY );
	}
}

void BaseView_placeTileSeries( BaseView* this, s16 x, s16 y, s16 w, s16 h, u8 pal, u16 tileIndex, bool autoInc ) {
	u8 i = 0, j = 0;

	void ( *placeTile )( struct BaseView*, s16, s16, u8, u16, bool, bool ) = FUNCTIONS( BaseView, BaseView, this )->placeTile;

	for( j = 0; j != h; j++ ) {
		for( i = 0; i != w; i++ ) {
			placeTile( this, x + i, y + j, pal, tileIndex, FALSE, FALSE );

			if( autoInc == TRUE ) {
				tileIndex++;
			}
		}
	}
}

void BaseView_listenToView( BaseView* this, BaseView* target, EventManager_EventKey event, EventManager_Callback callback ) {
	EventManager* targetEventManager = target->events;

	if( targetEventManager != NULL ) {
		EventManager_registerEvent( targetEventManager, this, event, callback );
	}
}

void BaseView_stopListeningView( BaseView* this, BaseView* target, EventManager_EventKey event ) {
	EventManager* targetEventManager = target->events;

	if( targetEventManager != NULL ) {
		EventManager_unregisterEvent( targetEventManager, this, event );
	}
}

void BaseView_trigger( BaseView* this, EventManager_EventKey event, void* payload ) {
	if( this->events != NULL ) {
		EventManager_trigger( this->events, event, payload );
	}
}

bool BaseView_checkTileBoundary( BaseView* this, s16 x, s16 y ) {

	// Check self
	s16 boundaryX = this->absX + this->width - 1;
	s16 boundaryY = this->absY + this->height - 1;

	bool result = ( x >= this->absX && y >= this->absY && x <= boundaryX && y <= boundaryY ) ? TRUE : FALSE;

	if( result == FALSE || this->parent == NULL ) {
		// If we hit the base element, or if the result evaluated to FALSE, do not check any longer and return the result
		return result;
	} else {
		return FUNCTIONS( BaseView, BaseView, this->parent )->checkTileBoundary( this->parent, x, y );
	}
}

bool BaseView_isView( void* view ) {
	BaseView* asView = ( BaseView* ) view;

	return asView->id == BaseView_isView_ID;
}

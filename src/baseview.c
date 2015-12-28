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
#include <vdpmanager.h>
#include <log.h>

static u16 BaseView_isView_ID = 0;
static u16 BaseView_isViewByTag_TAG = 0x0000;
static BaseView* BaseView_isInstance_INSTANCE = NULL;

BaseView_vtable BaseView_table = {
	BaseView_dtor,
	BaseView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_placeTile,
	BaseView_placeTileSeries,

	BaseView_listenToView,
	BaseView_stopListeningView,
	BaseView_trigger,

	BaseView_getChildById,
	BaseView_getChildByTag,
	BaseView_removeChild,

	BaseView_checkTileBoundary
};

void BaseView_ctor( BaseView* this, s16 x, s16 y, s16 width, s16 height ) {
	this->id = Romble_getUniqueId();
	this->tag = 0x0000;

	this->functions = &BaseView_table;

	this->absX = 0;
	this->absY = 0;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;

	this->numChildren = 0;

	this->children = NULL;
	this->parent = NULL;
	this->events = NULL;
}

void BaseView_dtor( BaseView* this ) {
	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "dtor called for BaseView %p", this );

	if( this->events != NULL ) {
		EventManager_dtor( this->events );
	}

	// Destroy & free all children of this view
	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "%p is removing each of its children...", this );
	LinkedListNode_each( this->children, BaseView_freeInstance );
	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Done removing children for %p!", this );
	// Destroy the list of children itself
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
		this->absX = this->x + this->parent->absX;
		this->absY = this->y + this->parent->absY;
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
		LinkedListNode_ctor( this->children );
		this->children->data = childView;
	} else {
		// Toss this at the end of the list if there's already a node
		LinkedListNode_insertEnd( this->children, childView );
	}

	childView->parent = this;
	this->numChildren++;
}

void BaseView_placeTile( BaseView* this, s16 plane, s16 x, s16 y, u8 pal, u16 tileIndex, bool flipV, bool flipH ) {
	s16 absX = x + this->absX;
	s16 absY = y + this->absY;

	VDP_setTileMapXY( plane, TILE_ATTR_FULL( pal, PRIORITY_LOW, flipV, flipH, tileIndex ), absX, absY );
}

void BaseView_placeTileSeries( BaseView* this, s16 plane, s16 x, s16 y, s16 w, s16 h, u8 pal, u16 tileIndex, bool autoInc ) {
	// Use VDP_fillTileMapRect/VDP_fillTileMapRectInc for speed. This won't check within the bounds for a view,
	// but that costs too much computation for too little benefit on this platform.
	s16 absX = x + this->absX;
	s16 absY = y + this->absY;

	if( autoInc == TRUE ) {
		VDP_fillTileMapRectInc( plane, TILE_ATTR_FULL( pal, PRIORITY_LOW, FALSE, FALSE, tileIndex ), absX, absY, w, h );
	} else {
		VDP_fillTileMapRect( plane, TILE_ATTR_FULL( pal, PRIORITY_LOW, FALSE, FALSE, tileIndex ), absX, absY, w, h );
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

BaseView* BaseView_getChildById( BaseView* this, u16 id ) {
	// Let the LinkedListNode do the work
	BaseView_isView_ID = id;
	return LinkedListNode_findData( this->children, BaseView_isView );
}

BaseView* BaseView_getChildByTag( BaseView* this, u16 tag ) {
	// Let the LinkedListNode do the work
	BaseView_isViewByTag_TAG = tag;
	return LinkedListNode_findData( this->children, BaseView_isViewByTag );
}

void BaseView_removeChild( BaseView* this, BaseView* childInstance, bool rerender ) {

	if( childInstance != NULL && this->children != NULL ) {
		Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "View %p is removing child %p", this, childInstance );

		// Pass to LinkedListNode methods
		// FALSE for removeAll - there should be only one instance at a time in the list of child views, if not, there's a bug elsewhere
		BaseView_isInstance_INSTANCE = childInstance;
		LinkedListNode_remove( &( this->children ), BaseView_isInstance, FALSE );

		Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Child %p removed from this->children, preparing to destroy...", childInstance );

		// Destroy childInstance (ask this object to clean up after itself and free objects it may own)
		FUNCTIONS( BaseView, BaseView, childInstance )->destroy( childInstance );

		Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Freeing ram at %p...", childInstance );

		// Free the memory located at childInstance - remember, all views should be on the heap!
		Romble_free_d( childInstance, FILE_LINE() );

		Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Re-rendering %p...", this );

		// Redraw the parent view
		if( rerender == TRUE ) {
			FUNCTIONS( BaseView, BaseView, this )->render( this );
		}
	} else {
		Log_fmessage( Log_Level_WARNING, FILE_LINE(), "Could not remove child %p: No children exist for %p or you have passed a null pointer.", childInstance, this );
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

bool BaseView_isViewByTag( void* instance ) {
	BaseView* view = ( BaseView* ) instance;

	return view->tag == BaseView_isViewByTag_TAG;
}

bool BaseView_isInstance( void* instance ) {
	BaseView* view = ( BaseView* ) instance;

	return view == BaseView_isInstance_INSTANCE;
}

void BaseView_freeInstance( void* instance ) {
	// Call the destructor and free the view instance
	BaseView* view = instance;

	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Destroying %p (vtable function %p)...", view, FUNCTIONS( BaseView, BaseView, view )->destroy );
	FUNCTIONS( BaseView, BaseView, view )->destroy( view );
	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "Freeing %p...", view );
	Romble_free_d( view, FILE_LINE() );
}

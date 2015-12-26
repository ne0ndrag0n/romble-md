#include <simpletextview.h>
#include <baseview.h>
#include <string.h>
#include <stdlib.h>
#include <vdp_bg.h>
#include <res/lang.h>
#include <res/globals.h>
#include <utility.h>
#include <lang.h>
#include <romble.h>
#include <vdpmanager.h>
#include <log.h>

SimpleTextView_vtable SimpleTextView_table = {
	SimpleTextView_dtor,
	SimpleTextView_render,
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

	BaseView_checkTileBoundary,

	SimpleTextView_setText
};

void SimpleTextView_ctor( SimpleTextView* this, char* text, s16 x, s16 y, bool copy ) {
	BaseView_ctor( ( BaseView* ) this, x, y, 0, 1 );
	CLASS( BaseView, this )->functions = &SimpleTextView_table;

	this->text = NULL;
	this->buffer = NULL;
	FUNCTIONS( SimpleTextView, BaseView, this )->setText( this, text, copy );
}

void SimpleTextView_dtor( SimpleTextView* this ) {
	Log_fmessage( Log_Level_DEBUG, FILE_LINE(), "dtor called for SimpleTextView %p", this );

	Romble_free_d( this->buffer, FILE_LINE() );
	this->buffer = NULL;

	this->text = NULL;

	BaseView_dtor( ( BaseView* ) this );
}

void SimpleTextView_render( SimpleTextView* this ) {
	BaseView* super = ( BaseView* ) this;

	FUNCTIONS( SimpleTextView, BaseView, this )->position( super );

	void ( *placeTile )( struct BaseView*, s16, s16, s16, u8, u16, bool, bool ) = FUNCTIONS( SimpleTextView, BaseView, this )->placeTile;

	// Place this->text character by character (visibility verification will be done each step of the way)
	size_t i;
	size_t len = strlen( this->text );
	for( i = 0; i != len; i++ ) {
		placeTile( super, VDPManager_LAYER_TRANSPARENT, i, 0, PAL0, TILE_FONTINDEX + ( this->text[ i ] - 32 ), FALSE, FALSE );
	}
}

void SimpleTextView_setText( SimpleTextView* this, char* text, bool copy ) {
	u8 width = strlen( text );

	// Whether you're copying a string or using a string byref, always free + null the buffer. On calling this function,
	// nothing in this->buffer will ever be valid: if you're using byref string (located in ROM), this is no good. If you're
	// copying a new string, it will be overwritten.
	Romble_free_d( this->buffer, FILE_LINE() );
	this->buffer = NULL;

	if( copy == TRUE ) {
		// Copy text into this->buffer
		// Restrict the buffer size
		if( width > SimpleTextView_MAXIMUM_BUFFER_SIZE ) {
			width = SimpleTextView_MAXIMUM_BUFFER_SIZE;
		}

		this->buffer = Romble_alloc_d( width * sizeof( char ) + 1, TRUE, FILE_LINE() );
		this->buffer[ 0 ] = 0;
		strncat( this->buffer, text, width );

		// this->text should NEVER have free called directly on it; it only ever points
		// to other usable memory (ROM or RAM)
		this->text = this->buffer;
	} else {
		// Set the pointer direct!
		this->text = text;
	}

	CLASS( BaseView, this )->width = width;
}

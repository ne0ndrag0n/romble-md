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

SimpleTextView_vtable SimpleTextView_table = {
	SimpleTextView_dtor,
	SimpleTextView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary,

	SimpleTextView_setText
};

void SimpleTextView_ctor( SimpleTextView* this, char* text, s16 x, s16 y ) {
	BaseView_ctor( ( BaseView* ) this, x, y, 0, 1 );
	this->super.functions = &SimpleTextView_table;

	FUNCTIONS( SimpleTextView, BaseView, this )->setText( this, text );
}

void SimpleTextView_dtor( SimpleTextView* this ) {
	Romble_secureFree( ( void* ) &( this->text ) );
	this->text = NULL;

	BaseView_dtor( ( BaseView* ) this );
}

void SimpleTextView_render( SimpleTextView* this ) {
	BaseView* super = ( BaseView* ) this;

	FUNCTIONS( SimpleTextView, BaseView, this )->position( super );

	void ( *placeTile )( struct BaseView*, s16, s16, u8, u16, bool, bool ) = FUNCTIONS( SimpleTextView, BaseView, this )->placeTile;

	// Place this->text character by character (visibility verification will be done each step of the way)
	size_t i;
	size_t len = strlen( this->text );
	for( i = 0; i != len; i++ ) {
		placeTile( super, i, 0, PAL0, TILE_FONTINDEX + ( this->text[ i ] - 32 ), FALSE, FALSE );
	}
}

void SimpleTextView_setText( SimpleTextView* this, char* text ) {
	u8 width = strlen( text );

	this->text = Romble_alloc( width * sizeof( char ) + 1, TRUE );
	Romble_assert( this->text != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	strcpy( this->text, text );

	CLASS( BaseView, this )->width = width;
}

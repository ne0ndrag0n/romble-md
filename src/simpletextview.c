#include <simpletextview.h>
#include <baseview.h>
#include <string.h>
#include <stdlib.h>
#include <vdp_bg.h>
#include <res/lang.h>
#include <res/globals.h>
#include <utility.h>

SimpleTextView_vtable SimpleTextView_table = { 
	SimpleTextView_ctor, 
	SimpleTextView_dtor, 
	SimpleTextView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary
};

void SimpleTextView_ctor( SimpleTextView* this, char* text, s16 x, s16 y ) {
	u8 width = strlen( text );
	
	BaseView_ctor( ( BaseView* ) this, x, y, width, 1 );

	this->text = ( char * ) calloc( width + 1, sizeof( char ) );
	if( this->text ) {
		strcpy( this->text, text );
	} else {
		SYS_die( EXCEPTION_OUT_OF_MEMORY );
	}

	// Fix the incompatible pointer types warning
	this->super.functions = &SimpleTextView_table;
}

void SimpleTextView_dtor( SimpleTextView* this ) {
	free( this->text );

	BaseView_dtor( ( BaseView* ) this );
}

void SimpleTextView_render( SimpleTextView* this ) {
	BaseView* super = ( BaseView* ) this;

	super->functions->position( super );

	void ( *placeTile )( struct BaseView*, s16, s16, u8, u16, bool, bool ) = super->functions->placeTile;

	// Place this->text character by character (visibility verification will be done each step of the way)
	size_t i;
	size_t len = strlen( this->text );
	for( i = 0; i != len; i++ ) {
		placeTile( super, i, 0, PAL0, TILE_FONTINDEX + ( this->text[ i ] - 32 ), FALSE, FALSE );
	}
}


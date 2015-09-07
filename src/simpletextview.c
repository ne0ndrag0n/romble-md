#include <simpletextview.h>
#include <baseview.h>
#include <string.h>
#include <stdlib.h>
#include <vdp_bg.h>
#include <res/lang.h>
#include <res/globals.h>

SimpleTextView_vtable SimpleTextView_table = { 
	SimpleTextView_ctor, 
	SimpleTextView_dtor, 
	SimpleTextView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries
};

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y ) {
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

	BaseView_render( ( BaseView* ) this );

	VDP_drawText( this->text, super->absX, super->absY );
}


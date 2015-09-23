#include <testview.h>
#include <baseview.h>
#include <boxview.h>
#include <simpletextview.h>
#include <types.h>
#include <res/globals.h>
#include <romble.h>
#include <lang.h>
#include <stdlib.h>
#include <vdp.h>

TestView_vtable TestView_table = { 
	TestView_ctor, 
	TestView_dtor, 
	BaseView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary
};

void TestView_ctor( TestView* this, s16 x, s16 y, s16 width, s16 height ) {
	BaseView_ctor( ( BaseView* ) this, x, y, width, height );
	
	CLASS( BaseView, this )->functions = &TestView_table;
	
	this->hiBox = NULL;
	this->hiText = NULL;

	// Create boxes
	this->hiBox = malloc( sizeof( BoxView ) );
	Romble_assert( this->hiBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	
	this->hiText = malloc( sizeof( SimpleTextView ) );
	Romble_assert( this->hiText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	
	BoxView_ctor( this->hiBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 0, 10, 3 );
	SimpleTextView_ctor( this->hiText, "Say Hi", 1, 1 );
	
	// Add text to hiBox
	CLASS( BaseView, this->hiBox )->functions->addChildView( 
		CLASS( BaseView, this->hiBox ), 
		CLASS( BaseView, this->hiText ) 
	);
	
	// Add hiBox to child view
	CLASS( BaseView, this )->functions->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->hiBox )
	);

}

void TestView_dtor( TestView* this ) {
	FREE_AND_NULL( this->hiBox );
	FREE_AND_NULL( this->hiText );
}

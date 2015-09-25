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
#include <joymanager.h>
#include <joy.h>
#include <buttonview.h>

TestView_vtable TestView_table = {
	TestView_dtor,
	TestView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_setPlane,
	BaseView_placeTile,
	BaseView_placeTileSeries,
	BaseView_checkTileBoundary,

	TestView_setupChildren
};

void TestView_ctor( TestView* this, s16 x, s16 y, s16 width, s16 height ) {
	BaseView_ctor( ( BaseView* ) this, x, y, width, height );
	CLASS( BaseView, this )->functions = &TestView_table;

	FUNCTIONS( TestView, BaseView, this )->setupChildren( this );
}

void TestView_dtor( TestView* this ) {

}

void TestView_render( TestView* this ) {
	BaseView_render( CLASS( BaseView, this ) );

	FUNCTIONS( ButtonView, BaseView, this->button1 )->setClickable( this->button1, TRUE );
	JoyManager_setDefaultCurrentElement( joyManager );
}

void TestView_setupChildren( TestView* this ) {
	this->button1 = calloc( 1, sizeof( ButtonView ) );
	Romble_assert( this->button1 != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	ButtonView_ctor( this->button1, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 0, 10 );

	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->button1 )
	);

	FUNCTIONS( ButtonView, BaseView, this->button1 )->setText( this->button1, "Ok", FALSE );
}

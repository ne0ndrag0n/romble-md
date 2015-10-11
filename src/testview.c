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
#include <image.h>
#include <gifimage.h>
#include <sizedarray.h>

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

	//FUNCTIONS( ButtonView, BaseView, this->button1 )->setClickable( this->button1, TRUE );
	//JoyManager_setDefaultCurrentElement( joyManager );

	// Let's put some image test shit here
	SizedArray testGif = {
		ExampleGif,
		69
	};
	GifImage* test;
	NEW_OBJECT( GifImage, test );
	FUNCTIONS( Image, Image, test )->loadData( CLASS( Image, test ), &testGif );
	FUNCTIONS( GifImage, Image, test )->getVDPTiles( test, TRUE );
}

void TestView_setupChildren( TestView* this ) {
	NEW_OBJECT( ButtonView, this->button1, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 0, 10 );

	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->button1 )
	);

	FUNCTIONS( ButtonView, BaseView, this->button1 )->setText( this->button1, "Image", FALSE );
}

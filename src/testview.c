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
#include <sizedarray.h>
#include <bit_trie.h>
#include <utility.h>
#include <vdpmanager.h>
#include <tags.h>

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

	// Determine this->buttonStyle by searching for tile tag TILES_BOX_DRAWING_BETA
	// If it's not in the VDPManager, load it into the VDPManager
	// TILES_BOX_DRAWING_BETA uses the system palette
	this->buttonStyle = VDPManager_getTilesByTag( vdpManager, TILES_BOX_DRAWING_BETA );
	if( this->buttonStyle == VDPManager_INDEX_NULL ) {
		// Need to load the tiles
		this->buttonStyle = VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, TILES_BOX_DRAWING_BETA );
	}

	FUNCTIONS( TestView, BaseView, this )->setupChildren( this );
}

void TestView_dtor( TestView* this ) {

}

void TestView_render( TestView* this ) {
	BaseView_render( CLASS( BaseView, this ) );

	// Refresh the JoyManager and prepare it for a new set of clickables
	JoyManager_unregisterAll( joyManager );
	FUNCTIONS( ButtonView, BaseView, this->button1 )->setClickable( this->button1, TRUE );
	JoyManager_setDefaultCurrentElement( joyManager );
}

void TestView_setupChildren( TestView* this ) {
	NEW_OBJECT( ButtonView, this->button1, this->buttonStyle, 0, 0, 0, 10 );

	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->button1 )
	);

	FUNCTIONS( ButtonView, BaseView, this->button1 )->setText( this->button1, "Test It!", FALSE );
}

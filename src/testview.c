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

	BaseView_listenToView,
	BaseView_stopListeningView,
	BaseView_trigger,

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
	FUNCTIONS( ButtonView, BaseView, this->hi )->setClickable( this->hi, TRUE );
	FUNCTIONS( ButtonView, BaseView, this->bye )->setClickable( this->bye, TRUE );
	FUNCTIONS( ButtonView, BaseView, this->obscenity )->setClickable( this->obscenity, TRUE );
	FUNCTIONS( ButtonView, BaseView, this->clear )->setClickable( this->clear, TRUE );
	JoyManager_setDefaultCurrentElement( joyManager );
}

void TestView_setupChildren( TestView* this ) {
	NEW_OBJECT( ButtonView, this->hi, this->buttonStyle, 0, 0, 0, 10 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->hi ) );
	FUNCTIONS( ButtonView, BaseView, this->hi )->setText( this->hi, "Say Hi", FALSE );

	NEW_OBJECT( ButtonView, this->bye, this->buttonStyle, 0, 0, 4, 10 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->bye ) );
	FUNCTIONS( ButtonView, BaseView, this->bye )->setText( this->bye, "Say Bye", FALSE );

	NEW_OBJECT( ButtonView, this->obscenity, this->buttonStyle, 0, 0, 8, 10 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->obscenity ) );
	FUNCTIONS( ButtonView, BaseView, this->obscenity )->setText( this->obscenity, "OH SHIT!", FALSE );

	NEW_OBJECT( ButtonView, this->displayedText, this->buttonStyle, 0, 12, 4, 20 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->displayedText ) );
	FUNCTIONS( ButtonView, BaseView, this->displayedText )->setText( this->displayedText, "(Press any button)", FALSE );

	NEW_OBJECT( ButtonView, this->clear, this->buttonStyle, 0, 12, 8, 20 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->clear ) );
	FUNCTIONS( ButtonView, BaseView, this->clear )->setText( this->clear, "Clear Text", FALSE );

	NEW_OBJECT( SimpleTextView, this->textView, "Romble Event Tester", 12, 1 );
	FUNCTIONS( TestView, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, this->textView ) );
}

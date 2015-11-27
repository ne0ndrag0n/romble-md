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

	// This is only a test
	BitwiseTrieNode* testTrie = Romble_alloc_d( sizeof( BitwiseTrieNode ), TRUE, FILE_LINE() );
	BitwiseTrieNode_ctor( testTrie, FALSE );
	char* test = "Test case 1";
	char* test2 = "Test case 2";
	BitwiseTrieNode_insert( testTrie, 32, test );
	BitwiseTrieNode_insert( testTrie, 128, test2 );
	Debug_print( ( char* ) BitwiseTrieNode_get( testTrie, 32 ) );
	Debug_print( ( char* ) BitwiseTrieNode_get( testTrie, 128 ) );
}

void TestView_setupChildren( TestView* this ) {
	NEW_OBJECT( ButtonView, this->button1, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 0, 10 );

	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->button1 )
	);

	FUNCTIONS( ButtonView, BaseView, this->button1 )->setText( this->button1, "Test It!", FALSE );
}

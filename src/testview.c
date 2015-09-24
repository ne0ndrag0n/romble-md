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

TestView_vtable TestView_table = {
	TestView_ctor,
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
	FREE_AND_NULL( this->hiBox )
	FREE_AND_NULL( this->hiText )
}

void TestView_render( TestView* this ) {
	BaseView_render( CLASS( BaseView, this ) );

	JoyManager_registerElement(
		joyManager,
		CLASS( BaseView, this->hiBox )->absX,
		CLASS( BaseView, this->hiBox )->absY,
		CLASS( BaseView, this->hiBox )->width,
		CLASS( BaseView, this->hiBox )->height,
		NULL
	);
	JoyManager_registerElement(
		joyManager,
		CLASS( BaseView, this->byeBox )->absX,
		CLASS( BaseView, this->byeBox )->absY,
		CLASS( BaseView, this->byeBox )->width,
		CLASS( BaseView, this->byeBox )->height,
		NULL
	);
	JoyManager_registerElement(
		joyManager,
		CLASS( BaseView, this->clearBox )->absX,
		CLASS( BaseView, this->clearBox )->absY,
		CLASS( BaseView, this->clearBox )->width,
		CLASS( BaseView, this->clearBox )->height,
		NULL
	);
	JoyManager_registerElement(
		joyManager,
		CLASS( BaseView, this->surpriseBox )->absX,
		CLASS( BaseView, this->surpriseBox )->absY,
		CLASS( BaseView, this->surpriseBox )->width,
		CLASS( BaseView, this->surpriseBox )->height,
		NULL
	);

	joyManager->currentElement = joyManager->registeredElements[ CLASS( BaseView, this->hiBox )->absX ][ CLASS( BaseView, this->hiBox )->absY ];
}

void TestView_setupChildren( TestView* this ) {
	this->hiBox = calloc( 1, sizeof( BoxView ) );
	Romble_assert( this->hiBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	BoxView_ctor( this->hiBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 0, 10, 3 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->hiBox )
	);

	this->hiText = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->hiText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->hiText, "Say Hi", 1, 1 );
	FUNCTIONS( BoxView, BaseView, this->hiBox )->addChildView(
		CLASS( BaseView, this->hiBox ),
		CLASS( BaseView, this->hiText )
	);

	this->byeBox = calloc( 1, sizeof( BoxView ) );
	Romble_assert( this->byeBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	BoxView_ctor( this->byeBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 0, 4, 10, 3 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->byeBox )
	);

	this->byeText = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->byeText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->byeText, "Say Bye", 1, 1 );
	FUNCTIONS( BoxView, BaseView, this->byeBox )->addChildView(
		CLASS( BaseView, this->byeBox ),
		CLASS( BaseView, this->byeText )
	);

	this->yourMessageLabel = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->yourMessageLabel != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->yourMessageLabel, "Your Message:", 15, 2 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->yourMessageLabel )
	);

	this->messageBox = calloc( 1, sizeof( BoxView ) );
	Romble_assert( this->messageBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	BoxView_ctor( this->messageBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 15, 3, 13, 3 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->messageBox )
	);

	this->messageText = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->messageText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->messageText, "Select One", 1, 1 );
	FUNCTIONS( BoxView, BaseView, this->messageBox )->addChildView(
		CLASS( BaseView, this->messageBox ),
		CLASS( BaseView, this->messageText )
	);


	this->clearBox = calloc( 1, sizeof( BoxView ) );
	Romble_assert( this->clearBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	BoxView_ctor( this->clearBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 5, 10, 15, 3 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->clearBox )
	);

	this->clearText = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->clearText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->clearText, "Clear Text", 1, 1 );
	FUNCTIONS( BoxView, BaseView, this->clearBox )->addChildView(
		CLASS( BaseView, this->clearBox ),
		CLASS( BaseView, this->clearText )
	);

	this->surpriseBox = calloc( 1, sizeof( BoxView ) );
	Romble_assert( this->surpriseBox != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	BoxView_ctor( this->surpriseBox, TILE_USERINDEX, BOX_DRAWING_INDEX, 5, 13, 15, 3 );
	FUNCTIONS( TestView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->surpriseBox )
	);

	this->surpriseText = calloc( 1, sizeof( SimpleTextView ) );
	Romble_assert( this->surpriseText != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	SimpleTextView_ctor( this->surpriseText, "Surprise!", 1, 1 );
	FUNCTIONS( BoxView, BaseView, this->surpriseBox )->addChildView(
		CLASS( BaseView, this->surpriseBox ),
		CLASS( BaseView, this->surpriseText )
	);
}

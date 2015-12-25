#include <buttonview.h>
#include <simpletextview.h>
#include <boxview.h>
#include <lang.h>
#include <stdlib.h>
#include <romble.h>
#include <res/globals.h>
#include <utility.h>
#include <joymanager.h>
#include <log.h>
#include <stdio.h>
#include <baseview.h>
#include <eventmanager.h>
#include <tags.h>

ButtonView_vtable ButtonView_table = {
	BaseView_dtor,
	BoxView_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_placeTile,
	BaseView_placeTileSeries,

	BaseView_listenToView,
	BaseView_stopListeningView,
	BaseView_trigger,

	BaseView_checkTileBoundary,
	ButtonView_setText,
	ButtonView_setClickable,
	ButtonView_onClick
};

void ButtonView_ctor( ButtonView* this, u16 boxDrawingIndex, u16 fillTileIndex, s16 x, s16 y, s16 width ) {
	// A ButtonView is a BoxView of height 3 that contains a SimpleTextView
	BoxView_ctor( CLASS( BoxView, this ), boxDrawingIndex, fillTileIndex, x, y, width, 3 );
	CLASS( BaseView, this )->functions = &ButtonView_table;

	NEW_OBJECT( SimpleTextView, this->label, "Button", 1, 1, TRUE );

	FUNCTIONS( ButtonView, BaseView, this )->addChildView(
		CLASS( BaseView, this ),
		CLASS( BaseView, this->label )
	);

	// Make use of the event manager pointer
	CLASS( BaseView, this )->events = Romble_alloc_d( sizeof( EventManager ), TRUE, FILE_LINE() );
	EventManager_ctor( CLASS( BaseView, this )->events );
}

void ButtonView_dtor( ButtonView* this ) {
	BaseView_dtor( CLASS( BaseView, this ) );

	FUNCTIONS( SimpleTextView, BaseView, this->label )->destroy( this->label );
}

void ButtonView_setText( ButtonView* this, char* label, bool render, bool copy ) {
	FUNCTIONS( SimpleTextView, BaseView, this->label )->setText( this->label, label, copy );

	if( render == TRUE ) {
		FUNCTIONS( SimpleTextView, BaseView, this->label )->render( this->label );
	}
}

// Button must be positioned in order to set clickable properly
void ButtonView_setClickable( ButtonView* this, bool clickable ) {
	if( clickable == TRUE ) {
		JoyManager_registerElement(
			joyManager,
			CLASS( BaseView, this )->absX,
			CLASS( BaseView, this )->absY,
			CLASS( BaseView, this )->width,
			CLASS( BaseView, this )->height,
			this,
			FUNCTIONS( ButtonView, BaseView, this )->onClick
		);
	} else {
		// Unregister element
		JoyManager_unregisterElement(
			joyManager,
			CLASS( BaseView, this )->absX,
			CLASS( BaseView, this )->absY
		);
	}
}

void ButtonView_onClick( void* instance, u16 button ) {
	ButtonView* this = instance;

	FUNCTIONS( ButtonView, BaseView, this )->trigger( CLASS( BaseView, this ), EVENT_CLICK, this );
}

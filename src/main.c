#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>
#include <simpletextview.h>
#include <boxview.h>
#include <utility.h>
#include <res/globals.h>
#include <vdp_pal.h>
#include <vdp_tile.h>
#include <romble.h>
#include <joymanager.h>

int main( void ) {

	Romble_init();

	BaseView root;
	BaseView_ctor( &root, 0, 0, 40, 28 );

	BoxView childView1;
	BoxView_ctor( &childView1, TILE_USERINDEX, BOX_DRAWING_INDEX, 2, 2, 10, 3 );

	BoxView childView2;
	BoxView_ctor( &childView2, TILE_USERINDEX, BOX_DRAWING_INDEX, 5, 6, 10, 3 );

	BoxView childView3;
	BoxView_ctor( &childView3, TILE_USERINDEX, BOX_DRAWING_INDEX, 2, 10, 10, 3 );

	SimpleTextView text1;
	SimpleTextView_ctor( &text1, "Option 1", 1, 1 );

	SimpleTextView text2;
	SimpleTextView_ctor( &text2, "Option 2", 1, 1 );

	SimpleTextView text3;
	SimpleTextView_ctor( &text3, "Option 3", 1, 1 );

	childView1.super.functions->addChildView( ( BaseView* ) &childView1, ( BaseView* ) &text1 );
	childView2.super.functions->addChildView( ( BaseView* ) &childView2, ( BaseView* ) &text2 );
	childView3.super.functions->addChildView( ( BaseView* ) &childView3, ( BaseView* ) &text3 );

	root.functions->addChildView( &root, ( BaseView* ) &childView1 );
	root.functions->addChildView( &root, ( BaseView* ) &childView2 );
	root.functions->addChildView( &root, ( BaseView* ) &childView3 );

	root.functions->render( &root );

	BaseView* box1 = ( BaseView* ) &childView1;
	JoyManager_registerElement( joyManager, box1->absX, box1->absY, box1->width, box1->height );
	
	BaseView* box2 = ( BaseView* ) &childView2;
	JoyManager_registerElement( joyManager, box2->absX, box2->absY, box2->width, box2->height );
	
	BaseView* box3 = ( BaseView* ) &childView3;
	JoyManager_registerElement( joyManager, box3->absX, box3->absY, box3->width, box3->height );
	
	joyManager->currentElement = joyManager->registeredElements[ box1->absX ][ box1->absY ];
	JoyManager_displayCursor( joyManager, TRUE );

	Debug_print( "Ok" );

	while(1);
}

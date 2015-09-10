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

int main( void ) {

	Romble_init();

	BaseView root;
	BaseView_ctor( &root, 0, 0, 40, 28 );

	BoxView childView;
	BoxView_ctor( &childView, TILE_USERINDEX, 5, 2, 2, 12, 12 );

	BoxView test;
	BoxView_ctor( &test, TILE_USERINDEX, 5, 2, 2, 12, 12 );

	BoxView anotherTest;
	BoxView_ctor( &anotherTest, TILE_USERINDEX, 5, 2, 2, 32, 32 );

	root.functions->addChildView( &root, ( BaseView* ) &childView );
	childView.super.functions->addChildView( ( BaseView* ) &childView, ( BaseView* ) &test );
	test.super.functions->addChildView( ( BaseView* ) &test, ( BaseView* ) &anotherTest );
	root.functions->render( &root );

	VDP_drawText( "Ok", 0, 0 );

	while(1);
}

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
	BoxView_ctor( &childView, TILE_USERINDEX, 3, 3, 20, 20 );

	// Nested boxviews!
	BoxView nested;
	BoxView_ctor( &nested, TILE_USERINDEX, 1, 1, 10, 5 );

	nested.super.functions->addChildView( ( BaseView* ) &childView, ( BaseView* ) &nested );
	root.functions->addChildView( &root, ( BaseView* ) &childView );
	root.functions->render( &root );

	VDP_drawText( "012345678901234567890", 0, 0 );

	while(1);
}

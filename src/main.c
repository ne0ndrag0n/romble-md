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

	SimpleTextView text;
	SimpleTextView_ctor( &text, "This should overflow about now", -2, 2 );

	root.functions->addChildView( &root, ( BaseView* ) &childView );
	childView.super.functions->addChildView( ( BaseView* ) &childView, ( BaseView* ) &text );
	root.functions->render( &root );

	Debug_print( "Ok" );

	while(1);
}

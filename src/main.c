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
	BoxView_ctor( &childView, TILE_USERINDEX, 5, 0, 1, 20, 21 );

	root.functions->addChildView( &root, ( BaseView* ) &childView );
	root.functions->render( &root );

	while(1);
}

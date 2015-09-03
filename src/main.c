#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>

int main( void ) {
	VDP_resetScreen();
	VDP_resetSprites();

	// Let's try nesting two views
	// First create the root view
	BaseView root;
	BaseView_ctor( &root, 0, 0, 30, 18 );

	VDP_drawText( "ok", 0, 0 );
	while(1);
}

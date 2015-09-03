#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>
#include <simpletextview.h>

int main( void ) {
	VDP_resetScreen();
	VDP_resetSprites();

	// Let's try nesting two views
	// First create the root view
	BaseView root;
	BaseView_ctor( &root, 0, 0, 30, 18 );

	SimpleTextView textView;
	SimpleTextView_ctor( &textView, "Hi", 2, 2 );
	
	char debug[50];
	sprintf( debug, "%d", textView.super.functions->testa( &textView.super ) );

	VDP_drawText( "ok", 0, 0 );
	VDP_drawText( debug, 1, 1 );
	while(1);
}

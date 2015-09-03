#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>
#include <simpletextview.h>
#include <utility.h>

int main( void ) {
	VDP_resetScreen();
	VDP_resetSprites();

	// Let's try nesting two views
	// First create the root view
	BaseView root;
	BaseView_ctor( &root, 0, 0, 30, 18 );

	SimpleTextView textView;
	SimpleTextView_ctor( &textView, "Hi", 2, 2 );

	BaseView* textAsBase = ( BaseView* ) &textView;

	root.functions->addChildView( &root, ( BaseView* ) &textView );
	root.functions->render( &root, FALSE );

	VDP_drawText( "012345678901234567890", 0, 0 );

	while(1);
}

#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>
#include <simpletextview.h>
#include <utility.h>
#include <res/globals.h>
#include <vdp_pal.h>
#include <romble.h>

int main( void ) {

	Romble_init();

	// Let's try nesting two views
	// First create the root view
	BaseView root;
	BaseView_ctor( &root, 0, 0, 40, 28 );

	SimpleTextView textView;
	SimpleTextView_ctor( &textView, "Hi", 2, 2 );

	root.functions->addChildView( &root, ( BaseView* ) &textView );
	root.functions->render( &root );

	VDP_drawText( "012345678901234567890", 0, 0 );

	while(1);
}

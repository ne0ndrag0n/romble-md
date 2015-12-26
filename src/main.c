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
#include <testview.h>
#include <res/globals.h>
#include <sram.h>
#include <log.h>

#ifdef UNIT_TEST_MODE
	#include <test/specrunner.h>
#endif

int main( void ) {

	#ifdef UNIT_TEST_MODE
		VDP_drawText( "Romble is compiled in Unit Test mode", 0, 0 );
		VDP_drawText( "Executing test cases...", 0, 1 );
		SpecRunner_test();
		VDP_drawText( "Done. Press RESET to test again.", 0, 3 );
		while(1);
	#endif

	Romble_init();

	BaseView* root;
	root = Romble_alloc_d( sizeof( BaseView ), TRUE, FILE_LINE() );
	BaseView_ctor( root, 0, 0, 40, 28 );

	TestView* test;
	test = Romble_alloc_d( sizeof( TestView ), TRUE, FILE_LINE() );
	TestView_ctor( test, 2, 2, 35, 25 );

	FUNCTIONS( BaseView, BaseView, root )->addChildView( root, CLASS( BaseView, test ) );
	FUNCTIONS( BaseView, BaseView, root )->render( root );

	Log_message( Log_Level_INFO, FILE_LINE(), "boot ok" );
	while(1);
}

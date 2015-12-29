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
#include <lang.h>
#include <testview.h>
#include <res/globals.h>
#include <sram.h>
#include <log.h>
#include <supervisor.h>
#include <supervisor_states.h>

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

	// Setup a Supervisor and change its state to Supervisor_State_BETATEST as an initial state
	Supervisor* supervisor = NULL;
	NEW_OBJECT( Supervisor, supervisor );
	FUNCTIONS( Supervisor, BaseView, supervisor )->changeState( supervisor, Supervisor_State_BETATEST );

	Log_message( Log_Level_INFO, FILE_LINE(), "boot ok" );
	while(1);
}

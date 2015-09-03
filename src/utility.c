#include <utility.h>
#include <vdp_bg.h>

u8 ROLL_COUNTER = 0;

void Debug_print( char* output ) {
	VDP_drawText( output, 0, ROLL_COUNTER++ );
}

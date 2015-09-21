#include <utility.h>
#include <vdp_bg.h>

u8 ROLL_COUNTER = 0;

void Debug_print( char* output ) {
	VDP_drawText( output, 0, ROLL_COUNTER++ );
}

inline s16 Utility_lerp( s16 p1, s16 p2, s16 percent ) {
	return ( p1 * percent ) / 100 + ( p2 * ( 100 - percent ) / 100 );
}

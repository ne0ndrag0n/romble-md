#include <utility.h>
#include <vdp_bg.h>
#include <stdarg.h>
#include <stdio.h>

u8 ROLL_COUNTER = 0;

void Debug_print( char* output ) {
	VDP_drawText( output, 0, ROLL_COUNTER++ );
}

inline void Debug_sprint( char* format, ... ) {
	va_list args;
	va_start( args, format );

	char string[ 100 ];
	vsprintf( string, format, args );
	Debug_print( string );

	va_end( args );
}

inline s16 Utility_lerp( s16 p1, s16 p2, s16 percent ) {
	return ( p1 * percent ) / 100 + ( p2 * ( 100 - percent ) / 100 );
}

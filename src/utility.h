#ifndef UTILITIES
#define UTILITIES

#include <types.h>

#define Utility_swapu16( x ) (((x) >> 8) | ((x) << 8))
#define Utility_swapu32( x ) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

// One per function! (vfsprintf is missing things from newlib so we're stuck with shit like this)
//define Debug_sprintf( format, ... ) char __output[ 100 ]; sprintf( __output, format, __VA_ARGS__ ); Debug_print

void Debug_print( char* output );
extern void Debug_sprint( char* format, ... );

extern s16 Utility_lerp( s16 p1, s16 p2, s16 percent );


#endif

#ifndef ROMBLE
#define ROMBLE

#include <types.h>

extern u16 BOX_DRAWING_INDEX;

void Romble_init();
u16 Romble_loadTiles( const u32* tiles, u16 run );

#endif

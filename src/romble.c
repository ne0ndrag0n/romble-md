#include <romble.h>
#include <types.h>
#include <vdp_pal.h>
#include <vdp_bg.h>
#include <res/globals.h>

void Romble_init() {
	VDP_resetScreen();
	VDP_resetSprites();
	
	TILES_USED = 0;

	VDP_setPalette( PAL0, StandardColours );

	// Placed directly at the beginning
	Romble_loadTiles( BoxDrawingCharacters, 3 );
}

u16 Romble_loadTiles( const u32* tiles, u16 run ) {
	VDP_loadTileData( tiles, TILE_USERINDEX + TILES_USED, run, TRUE );

	TILES_USED = TILES_USED + run;
}

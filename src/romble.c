#include <romble.h>
#include <types.h>
#include <vdp_pal.h>
#include <vdp_bg.h>
#include <res/globals.h>
#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys.h>

u16 BOX_DRAWING_INDEX;

void Romble_init() {
	VDP_resetScreen();
	VDP_resetSprites();
	
	TILES_USED = 0;

	VDP_setPalette( PAL0, StandardColours );

	// Placed directly at the beginning
	BOX_DRAWING_INDEX = Romble_loadTiles( BoxDrawingCharacters, 3 );

	joyManager = calloc( 1, sizeof( JoyManager ) );
	JoyManager_ctor( joyManager, 64, 64 );
}

u16 Romble_loadTiles( const u32* tiles, u16 run ) {
	u16 loadPoint = TILE_USERINDEX + TILES_USED;

	VDP_loadTileData( tiles, loadPoint, run, TRUE );

	TILES_USED = TILES_USED + run;

	return loadPoint;
}

void Romble_assert( bool condition, char* failMessage ) {
	if ( condition == FALSE ) {
		SYS_die( failMessage );
	}
}

#include <romble.h>
#include <types.h>
#include <vdp_pal.h>
#include <vdp_bg.h>
#include <res/globals.h>
#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys.h>
#include <lang.h>
#include <string.h>
#include <romble.h>
#include <utility.h>

u16 BOX_DRAWING_INDEX;
static u16 UNIQUE_ID = 0;

void Romble_init() {
	VDP_resetScreen();
	VDP_resetSprites();

	TILES_USED = 0;

	VDP_setPalette( PAL0, StandardColours );

	// Placed directly at the beginning
	BOX_DRAWING_INDEX = Romble_loadTiles( BoxDrawingCharacters, 3 );

	joyManager = Romble_alloc( 1 * sizeof( JoyManager ), TRUE );
	Romble_assert( joyManager != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	JoyManager_ctor( joyManager, 40, 28 );

	Debug_print( "Welcome to the Romble beta!" );
	Debug_print( "Romble v0.0.3a (c) 2015 oaktree Games and Novelties. All rights reserved." );
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

u16 Romble_getUniqueId() {
	return ++UNIQUE_ID;
}

void Romble_secureFree( void** pointer ) {
	Debug_sprint( "pointer = %p, *pointer = %p", pointer, *pointer );

	// Cannot dereference a null pointer!
	if( pointer != NULL ) {
		free( *pointer );

		*pointer = NULL;
	}
}

void* Romble_alloc( size_t size, bool clear ) {

	void* pointer = NULL;

	pointer = malloc( size );

	if( pointer != NULL ) {

		if( clear == TRUE ) {
			memset( pointer, 0, size );
		}

	}

	return pointer;
}

void* Romble_realloc( void* pointer, size_t newSize, bool clear ) {

	void* newPointer = NULL;

	newPointer = Romble_alloc( newSize, clear );

	// Copy contents
	if( newPointer != NULL ) {
		memcpy( pointer, newPointer, newSize );
		free( pointer );
	}

	return newPointer;
}

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
#include <vdpmanager.h>

u16 BOX_DRAWING_INDEX;
static u16 UNIQUE_ID = 0;

void Romble_init() {
	VDP_resetScreen();
	VDP_resetSprites();

	TILES_USED = 0;

	VDP_setPalette( PAL0, StandardColours );

	// Placed directly at the beginning
	BOX_DRAWING_INDEX = Romble_loadTiles( BoxDrawingCharacters, 3 );

	joyManager = Romble_alloc_d( sizeof( JoyManager ), TRUE, FILE_LINE() );
	JoyManager_ctor( joyManager, 40, 28 );

	vdpManager = Romble_alloc_d( sizeof( VDPManager ), TRUE, FILE_LINE() );
	VDPManager_ctor( vdpManager );

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
	// Cannot dereference a null pointer!
	if( pointer != NULL ) {
		Romble_free( *pointer );

		*pointer = NULL;
	}
}

void Romble_free( void* pointer ) {
	free( pointer );
}

/**
 * Debug wrapper for romble secure free
 */
void Romble_secureFree_d( void** pointer, char* fileLine ) {
	#ifdef ALLOC_DEBUG_OUTPUT
		Debug_sprint( "ROMBLEFREE: %s passed pointer-to-pointer %p, dereferenced: %p", fileLine, pointer, *pointer );
	#endif

	Romble_secureFree( pointer );
}

/**
 * Debug wrapper for romble free
 */
void Romble_free_d( void* pointer, char* fileLine ) {
	#ifdef ALLOC_DEBUG_OUTPUT
		Debug_sprint( "ROMBLEFREE: %s passed pointer %p", fileLine, pointer );
	#endif

	Romble_free( pointer );
}

/**
 * Debug wrapper for romble alloc
 */
void* Romble_alloc_d( size_t size, bool clear, char* fileLine ) {
	void* result = Romble_alloc( size, clear );

	#ifdef ALLOC_DEBUG_OUTPUT
		Debug_sprint( "%s allocated %d bytes at pointer %p (clear: %d)", fileLine, size, result, clear );
	#endif

	if( result == NULL ) {
		char failure[ 50 ];
		strcat( failure, fileLine );
		strcat( failure, " " );
		strcat( failure, EXCEPTION_OUT_OF_MEMORY );
		SYS_die( failure );
	}

	return result;
}

/**
 * Debug wrapper for romble realloc
 */
void* Romble_realloc_d( void* pointer, size_t newSize, char* fileLine ) {
	void* result = Romble_realloc( pointer, newSize );

	#ifdef ALLOC_DEBUG_OUTPUT
		Debug_sprint( "%s reallocated %d bytes at pointer %p", fileLine, newSize, result );
	#endif

	if( result == NULL ) {
		char failure[ 50 ];
		strcat( failure, fileLine );
		strcat( failure, " " );
		strcat( failure, EXCEPTION_OUT_OF_MEMORY );
		SYS_die( failure );
	}

	return result;
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

void* Romble_realloc( void* pointer, size_t newSize ) {
	return realloc( pointer, newSize );
}

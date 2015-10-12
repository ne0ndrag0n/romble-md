#ifndef ROMBLE
#define ROMBLE

#include <types.h>
#include <res/globals.h>
#include <joymanager.h>
#include <stddef.h>

#define	DEBUG_ALLOC( size, clear ) Romble_alloc( size, clear ); Debug_sprint( "(%s, %d) requested %d bytes: clear? %d", __FILE__, __LINE__, size, clear );
#define DEBUG_REALLOC( ptr, size, clear ) Romble_realloc( ptr, size, clear ); Debug_sprint( "(%s, %d) reallocated %d bytes: clear? %d", __FILE__, __LINE__, size, clear );

extern u16 BOX_DRAWING_INDEX;

void Romble_init();
u16 Romble_loadTiles( const u32* tiles, u16 run );
void Romble_setJoyManager();
void Romble_assert( bool condition, char* failMessage );
u16 Romble_getUniqueId();
void Romble_secureFree( void** pointer );

// These alloc wrappers will allow us to diagnose heap corruption issues
void* Romble_alloc( size_t size, bool clear );
void* Romble_realloc( void* pointer, size_t newSize, bool clear );

#endif

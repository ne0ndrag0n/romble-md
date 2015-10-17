#ifndef ROMBLE
#define ROMBLE

#include <types.h>
#include <res/globals.h>
#include <joymanager.h>
#include <stddef.h>

extern u16 BOX_DRAWING_INDEX;

void Romble_init();
u16 Romble_loadTiles( const u32* tiles, u16 run );
void Romble_setJoyManager();
void Romble_assert( bool condition, char* failMessage );
u16 Romble_getUniqueId();
void Romble_secureFree( void** pointer );

void Romble_secureFree_d( void** pointer, char* fileLine );
void* Romble_alloc_d( size_t size, bool clear, char* fileLine );
void* Romble_realloc_d( void* pointer, size_t newSize, bool clear, char* fileLine );

// These alloc wrappers will allow us to diagnose heap corruption issues
void* Romble_alloc( size_t size, bool clear );
void* Romble_realloc( void* pointer, size_t newSize, bool clear );

#endif

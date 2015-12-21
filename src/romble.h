#ifndef ROMBLE
#define ROMBLE

#include <types.h>
#include <res/globals.h>
#include <joymanager.h>
#include <stddef.h>

void Romble_init();
void Romble_setJoyManager();
void Romble_assert( bool condition, char* failMessage );
u16 Romble_getUniqueId();
void Romble_free( void* pointer );
void Romble_free_d( void* pointer, char* fileLine );
void Romble_secureFree( void** pointer );

void Romble_secureFree_d( void** pointer, char* fileLine );
void* Romble_alloc_d( size_t size, bool clear, char* fileLine );
void* Romble_realloc_d( void* pointer, size_t newSize, char* fileLine );

// These alloc wrappers will allow us to diagnose heap corruption issues
void* Romble_alloc( size_t size, bool clear );
void* Romble_realloc( void* pointer, size_t newSize );

#endif

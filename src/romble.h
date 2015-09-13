#ifndef ROMBLE
#define ROMBLE

#include <types.h>
#include <res/globals.h>
#include <joymanager.h>

extern u16 BOX_DRAWING_INDEX;
extern JoyManager* joyManager;

void Romble_init();
u16 Romble_loadTiles( const u32* tiles, u16 run );
void Romble_setJoyManager();
void Romble_assert( bool condition, char* failMessage );

#endif

#ifndef GLOBALS
#define GLOBALS

#define FALSE 0
#define TRUE 1
#define PRIORITY_LOW 0
#define PRIORITY_HIGH 1

#define S( x ) #x
#define S_( x ) S( x )
#define S__LINE__ S_( __LINE__ )

#include <types.h>

typedef char bool;

extern u16 TILES_USED;

extern const u32 BoxDrawingCharacters[ 24 ];
extern const u16 StandardColours[ 16 ];


#endif

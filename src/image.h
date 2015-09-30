#ifndef IMAGE
#define IMAGE

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>

struct Image;

typedef void* Image_nativePalette;

typedef struct {
	void ( *destroy )( struct Image* );
	void ( *loadData )( struct Image*, SizedArray* items );
	SizedArray* ( *getVDPTiles )( struct Image*, bool );
} Image_vtable;

typedef struct Image {
	SizedArray* imageData;
	SizedArray* vdpTiles;
	SizedArray* nativePalette;
	u16* palette;
	u16 width;
	u16 height;

	vtable functions;
} Image;

void Image_ctor( Image* this );
void Image_dtor( Image* this );
void Image_loadData( Image* this, SizedArray* file );
SizedArray* Image_getVDPTiles( Image* this, bool keep );

#endif

#ifndef GIFIMAGE
#define GIFIMAGE

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>
#include <image.h>

#define GifImage_GIF87a	"GIF87a"
#define GifImage_GIF89a	"GIF89a"

struct GifImage;

typedef u8* GifImage_nativePalette;

typedef struct {
	void ( *destroy )( struct GifImage* );
	void ( *loadData )( struct Image*, SizedArray* items );
	void ( *buildPalette )( struct GifImage*, GifImage_nativePalette );
	SizedArray* ( *getVDPTiles )( struct GifImage*, bool );
} GifImage_vtable;

typedef struct GifImage {
	Image super;

	char* gifRevision;
} GifImage;

void GifImage_ctor( GifImage* this );
void GifImage_dtor( GifImage* this );
void GifImage_buildPalette( GifImage* this, GifImage_nativePalette nativePalette );
SizedArray* GifImage_getVDPTiles( GifImage* this, bool keep );

bool GifImage_isGifImage( SizedArray* file );

#endif

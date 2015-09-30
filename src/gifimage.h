#ifndef GIFIMAGE
#define GIFIMAGE

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>
#include <image.h>

#define GifImage_GIF87a	"GIF87a"
#define GifImage_GIF89a	"GIF89a"

#define GifImage_PALETTE_PRESENT_MASK     0x80
#define GifImage_PALETTE_SIZE_MASK        0x07

struct GifImage;

typedef u8* GifImage_nativePalette;

typedef struct {
	void ( *destroy )( struct GifImage* );
	void ( *loadData )( struct Image*, SizedArray* items );
	SizedArray* ( *getVDPTiles )( struct GifImage*, bool );
} GifImage_vtable;

typedef struct GifImage {
	Image super;

	u8 backgroundPalIndex;
} GifImage;

void GifImage_ctor( GifImage* this );
void GifImage_dtor( GifImage* this );
SizedArray* GifImage_getVDPTiles( GifImage* this, bool keep );

bool GifImage_isGifImage( SizedArray* file );
void GifImage_buildPalette( GifImage* this, SizedArray* file, u8 packedField );

#endif

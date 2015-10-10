#ifndef GIFIMAGE
#define GIFIMAGE

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>
#include <image.h>

#define GifImage_GIF87a	"GIF87a"
#define GifImage_GIF89a	"GIF89a"

#define GifImage_EXTENSION				  0x21
#define GifImage_EXTENSION_CONTROL		  0xF9
#define GifImage_EXTENSION_COMMENT		  0xFE
#define GifImage_EXTENSION_PLAINTEXT	  0x01
#define GifImage_EXTENSION_APPLICATION	  0xFF

#define GifImage_IMAGE_SEPARATOR		  0x2C

#define GifImage_TRAILER				  0x3B

#define GifImage_PALETTE_PRESENT_MASK     0x80
#define GifImage_PALETTE_SIZE_MASK        0x07
#define GifImage_INTERLACE_MASK			  0x40

struct GifImage;

typedef struct {
	void ( *destroy )( struct GifImage* );
	void ( *loadData )( struct Image*, SizedArray* items );
	SizedArray* ( *getVDPTiles )( struct GifImage*, bool );
} GifImage_vtable;

typedef struct GifImage_ControlParameters {
	u8 packedField;
	u16 delayTime;
	u8 transparentIndex;
} GifImage_ControlParameters;

typedef struct GifImage_ImageDescriptor {
	u16 imageLeft;
	u16 imageTop;
	u16 imageWidth;
	u16 imageHeight;
	u8 options;
} GifImage_ImageDescriptor;

typedef struct GifImage {
	Image super;

	GifImage_ControlParameters controlParams;
	u8 backgroundPalIndex;
	bool imageDrawn;
} GifImage;

void GifImage_ctor( GifImage* this );
void GifImage_dtor( GifImage* this );
SizedArray* GifImage_getVDPTiles( GifImage* this, bool keep );

bool GifImage_isGifImage( SizedArray* file );
void GifImage_buildPalette( GifImage* this, SizedArray* file, u8 packedField );

void GifImage_loadControlParameters( GifImage* this, SizedArray* file );
// The magic happens here!
void GifImage_processImage( GifImage* this, SizedArray* file );
void GifImage_decompress( GifImage* this, SizedArray* compressedBlock, u8 minCodeSize, u32* lastPixelDecoded );

#endif

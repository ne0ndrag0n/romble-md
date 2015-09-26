#include <gifimage.h>
#include <image.h>
#include <types.h>
#include <res/globals.h>
#include <romble.h>

GifImage_vtable GifImage_table = {
	GifImage_dtor,
	Image_loadData,
	GifImage_buildPalette,
	GifImage_getVDPTiles
};

void GifImage_ctor( GifImage* this ) {
	Image_ctor( CLASS( Image, this ) );

	CLASS( Image, this )->functions = &GifImage_table;

	this->gifRevision = NULL;
};

void GifImage_dtor( GifImage* this ) {
	Image_dtor( CLASS( Image, this ) );

	Romble_secureFree( ( void* ) &( this->gifRevision ) );
};

void GifImage_buildPalette( GifImage* this, GifImage_nativePalette nativePalette ) {
	CLASS( Image, this )->nativePalette = nativePalette;
}

// Turn GIF image into VDP tiles
SizedArray* GifImage_getVDPTiles( GifImage* this, bool keep ) {
	SizedArray* vdpTiles = Image_getVDPTiles( CLASS( Image, this ), keep );

	// Return vdpTiles if they were already generated prior
	if( vdpTiles->length > 0 ) {
		return vdpTiles;
	}

	// Return null if there is no image data to process
	if( CLASS( Image, this )->imageData == NULL || CLASS( Image, this )->imageData->length ) {
		Romble_secureFree( ( void* ) &( vdpTiles ) );
		return NULL;
	}

	// Turn GIF image into VDP tiles
	return vdpTiles;
}

#include <gifimage.h>
#include <image.h>
#include <types.h>
#include <res/globals.h>
#include <romble.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <utility.h>

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
	SizedArray file = { NULL, 0 };

	// If there are no vdp tiles, create a container for VDP tiles
	if( vdpTiles == NULL ) {
		CLASS( Image, this )->vdpTiles = calloc( 1, sizeof( SizedArray ) );
		vdpTiles = CLASS( Image, this )->vdpTiles;
	}

	// Copy file information into a local variable
	if( CLASS( Image, this )->imageData != NULL &&
	    CLASS( Image, this )->imageData->items != NULL ) {

		file.length = CLASS( Image, this )->imageData->length;
		file.items = calloc( CLASS( Image, this )->imageData->length, sizeof( u8 ) );
		memcpy( file.items, CLASS( Image, this )->imageData->items, file.length );
	}

	if( GifImage_isGifImage( &file ) == TRUE ) {
		Debug_print( "It's a gif" );
	}

	return vdpTiles;
}

bool GifImage_isGifImage( SizedArray* file ) {
	char header[7] = { 0 };
	memcpy( header, file->items, 6 );

	// Increment the file pointer
	file->items = ( ( char* ) file->items ) + 6;
	file->length -= 6;

	return ( strcmp( header, GifImage_GIF87a ) || strcmp( header, GifImage_GIF89a ) );
}

#include <gifimage.h>
#include <image.h>
#include <types.h>
#include <res/globals.h>
#include <romble.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <utility.h>
#include <stdio.h>

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

	// Clone pointer to file - This method returns a null pointer if there is no file
	if( CLASS( Image, this )->imageData != NULL ) {
		file.items = CLASS( Image, this )->imageData->items;
		file.length = CLASS( Image, this )->imageData->length;
	} else {
		return NULL;
	}

	// If this->vdpTiles is not null and keep is true, simply return what we got
	// If keep is false, clear what we got
	if( vdpTiles != NULL ) {
		if( keep == TRUE ) {
			return vdpTiles;
		} else {
			Romble_secureFree( ( void * ) &( vdpTiles->items ) );
			vdpTiles->length = 0;
		}
	} else {
		vdpTiles = calloc( 1, sizeof( SizedArray ) );

		if( keep == TRUE ) {
			CLASS( Image, this )->vdpTiles = vdpTiles;
		}
	}

	if ( GifImage_isGifImage( &file ) == TRUE ) {
		Debug_print( "GIF89a" );
	}

	return vdpTiles;
}

bool GifImage_isGifImage( SizedArray* file ) {
	char header[7] = { 0 };
	memcpy( header, file->items, 6 );

	// Increment the file pointer
	file->items = ( ( char* ) file->items ) + 6;
	file->length -= 6;

	return ( strcmp( header, GifImage_GIF87a ) == 0 || strcmp( header, GifImage_GIF89a ) == 0 );
}

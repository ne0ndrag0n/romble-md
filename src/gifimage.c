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
#include <sizedarray.h>

GifImage_vtable GifImage_table = {
	GifImage_dtor,
	Image_loadData,
	GifImage_getVDPTiles
};

void GifImage_ctor( GifImage* this ) {
	Image_ctor( CLASS( Image, this ) );

	CLASS( Image, this )->functions = &GifImage_table;

	this->backgroundPalIndex = 0;
};

void GifImage_dtor( GifImage* this ) {
	Image_dtor( CLASS( Image, this ) );
};

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
		SizedArray_takeBytes( &file, &( CLASS( Image, this )->width ), 2 );
		CLASS( Image, this )->width = Utility_swapu16( CLASS( Image, this )->width );

		SizedArray_takeBytes( &file, &( CLASS( Image, this )->height ), 2 );
		CLASS( Image, this )->height = Utility_swapu16( CLASS( Image, this )->height );

		u8 packedField = 0;
		SizedArray_takeBytes( &file, &packedField, 1 );

		SizedArray_takeBytes( &file, &( this->backgroundPalIndex ), 1 );

		// Burn the aspect ratio as it's irrelevant
		file.items = ( ( char* ) file.items ) + 1;
		file.length--;

		GifImage_buildPalette( this, &file, packedField );
	}

	return vdpTiles;
}

bool GifImage_isGifImage( SizedArray* file ) {
	char header[7] = { 0 };
	SizedArray_takeBytes( file, header, 6 );

	return ( strcmp( header, GifImage_GIF87a ) == 0 || strcmp( header, GifImage_GIF89a ) == 0 );
}

void GifImage_buildPalette( GifImage* this, SizedArray* file, u8 packedField ) {
	bool palettePresent = ( packedField & GifImage_PALETTE_PRESENT_MASK ) >> 7;
	u8 paletteSize = ( packedField & GifImage_PALETTE_SIZE_MASK );
	u16 numPaletteEntries; // Needs to fit the number 256

	if( palettePresent == TRUE ) {
		// Conducts the operation 2^n+1
		numPaletteEntries =  1 << ( paletteSize + 1 );

		// Store the native palette as a u8 SizedArray - first clear out existing SizedArray
		if( CLASS( Image, this )->nativePalette != NULL ) {
			Romble_secureFree( ( void* ) &( CLASS( Image, this )->nativePalette->items ) );
			Romble_secureFree( ( void* ) &( CLASS( Image, this )->nativePalette ) );
		}

		// Allocate room for SizedArray, memcpy bytes into the SizedArray
		CLASS( Image, this )->nativePalette = calloc( 1, sizeof( SizedArray ) );
		// Each palette entry for GIF is three bytes
		CLASS( Image, this )->nativePalette->length = numPaletteEntries * 3;
		CLASS( Image, this )->nativePalette->items = calloc( CLASS( Image, this )->nativePalette->length, sizeof( u8 ) );
		SizedArray_takeBytes( file, CLASS( Image, this )->nativePalette->items, CLASS( Image, this )->nativePalette->length );

		// Determine what palette to generate based on this Image class's PaletteMode.
		switch( CLASS( Image, this )->paletteMode ) {
			case Image_PaletteMode_OCTREE:
				// Convert a non-indexed image or image > 16 entries.
				// Drop down to NATIVE_IMAGE if the image already has less than or equal to 16 indexed colours
				CLASS( Image, this )->paletteMode = Image_PaletteMode_OCTREE;
				break;
			case Image_PaletteMode_NATIVE_IMAGE:
				// Simply convert this image's native palette to Sega format if there are 16 or fewer palette entries.
				// Drop down to NEAREST_DEFAULT if the image is non-indexed or has > 16 palette entries.
				break;
			case Image_PaletteMode_NEAREST_DEFAULT:
				break;
		}
	}
}

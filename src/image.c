#include <image.h>
#include <sizedarray.h>
#include <types.h>
#include <romble.h>
#include <stdlib.h>
#include <res/globals.h>
#include <lang.h>

Image_vtable Image_table = {
	Image_dtor,
	Image_loadData,
	Image_getVDPTiles
};

void Image_ctor( Image* this ) {
	this->imageData = NULL;
	this->vdpTiles = NULL;

	this->width = 0;
	this->height = 0;

	this->nativePalette = NULL;
	this->palette = NULL;
	this->paletteMode = Image_PaletteMode_NATIVE_IMAGE;

	this->functions = &Image_table;
}

void Image_dtor( Image* this ) {
	if( this->imageData != NULL ) {
		Romble_secureFree( ( void * ) &( this->imageData->items ) );
	}
	Romble_secureFree( ( void* ) &( this->imageData ) );

	if( this->vdpTiles != NULL ) {
		Romble_secureFree( ( void* ) &( this->vdpTiles->items ) );
	}
	Romble_secureFree( ( void* ) &( this->vdpTiles ) );
}

void Image_loadData( Image* this, SizedArray* file ) {
	this->imageData = file;

	if( this->vdpTiles != NULL ) {
		free( this->vdpTiles->items );
		Romble_secureFree( ( void* ) &( this->vdpTiles ) );

		this->vdpTiles = NULL;
	}
}

SizedArray* Image_getVDPTiles( Image* this, bool keep ) {
	return this->vdpTiles;
}

u16* Image_RGBtoSega( SizedArray* rgbTriplets ) {
	// To convert an RGB component to SEGA:
	// 1) take component
	// 2) if lower nibble is < 8, round higher nibble down to lowest even number.
	//    if lower nibble is >= 8, round higher nibble up to nearest even number. if higher nibble is already E, leave it.
	// 3) store higher nibble
	if( rgbTriplets->length <= 0 ) {
		return NULL;
	}

	u16* segaPalette = calloc( rgbTriplets->length, sizeof( u16 ) );
	Romble_assert( segaPalette != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	size_t triplet, channel;
	u8 channelCol, upperNibble;
	u16 newComponentCol;

	// TODO: naive solution? try to improve
	for( triplet = 0; triplet < rgbTriplets->length; triplet+=3 ) {
		for( channel = triplet; channel < triplet + 3; channel++ ) {
			channelCol = ( ( u8* )rgbTriplets->items )[ channel ];
			upperNibble = ( channelCol & Image_U8_UPPER_NIBBLE ) >> 4;

			// Determine if a rounding is necessary on the upper RGB nibble
			if( ( channelCol & Image_U8_LOWER_NIBBLE ) > 8 ) {
				// Upper nibble requires rounding to retrieve Sega col
				if( upperNibble < Image_MAXIMUM_ROUNDABLE ) {
					// Round to nearest even number
					( upperNibble % 2 == 0 ) ? upperNibble+= 2 : upperNibble++;
				} else {
					// If the channel's upper nibble is 0x0F, send it down to 0x0E
					if ( upperNibble == Image_MAXIMUM_RGB_NIBBLE ) {
						upperNibble = Image_MAXIMUM_ROUNDABLE;
					}
				}
			}
			// else - No rounding required, use upperNibble with no change

			// Now, shift new Sega value nibble by ( 4 * ( ( channel % 3 ) + 1 ) ) - 4
			// then bitwise "OR" it onto channelCol
			// On the first run, this applies R to the Sega R component, G on the second one, B on the third
			newComponentCol = ( u16 ) upperNibble;
			segaPalette[ triplet ] |= newComponentCol << ( ( 4 * ( ( channel % 3 ) + 1 ) ) - 4 );
		}
	}

	return segaPalette;
}

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
	this->nativePalette = NULL;

	this->palette = NULL;
	this->paletteMode = Image_PaletteMode_NATIVE_IMAGE;

	this->imageStatus = Image_Status_UNBUILT;

	this->width = 0;
	this->height = 0;

	this->paddedWidth = 0;
	this->paddedHeight = 0;

	this->functions = &Image_table;
}

void Image_dtor( Image* this ) {
	if( this->imageData != NULL ) {
		Romble_secureFree_d( ( void * ) &( this->imageData->items ), FILE_LINE() );
	}
	Romble_secureFree_d( ( void* ) &( this->imageData ), FILE_LINE() );

	if( this->vdpTiles != NULL ) {
		Romble_secureFree_d( ( void* ) &( this->vdpTiles->items ), FILE_LINE() );
	}
	Romble_secureFree_d( ( void* ) &( this->vdpTiles ), FILE_LINE() );
}

void Image_loadData( Image* this, SizedArray* file ) {
	this->imageData = file;

	if( this->vdpTiles != NULL ) {
		Romble_secureFree_d( ( void* ) &( this->vdpTiles->items ), FILE_LINE() );
		Romble_secureFree_d( ( void* ) &( this->vdpTiles ), FILE_LINE() );
	}
}

SizedArray* Image_getVDPTiles( Image* this, bool keep ) {
	return this->vdpTiles;
}

/**
 * Behaviour is undefined for rgbTriplets->length greater than 16!
 */
u16* Image_RGBtoSega( SizedArray* rgbTriplets ) {
	// To convert an RGB component to SEGA:
	// 1) take component
	// 2) if lower nibble is < 8, round higher nibble down to lowest even number.
	//    if lower nibble is >= 8, round higher nibble up to nearest even number. if higher nibble is already E, leave it.
	// 3) store higher nibble
	if( rgbTriplets == NULL || rgbTriplets->length <= 0 ) {
		return NULL;
	}

	u16* segaPalette = Romble_alloc_d( Image_SEGA_PAL_SIZE * sizeof( u16 ), TRUE, FILE_LINE() );
	Romble_assert( segaPalette != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	size_t triplet, channel;
	u8 channelCol, upperNibble;
	u16 newComponentCol = 0;

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
				}
			}

			// else - No rounding required, use upperNibble with no change

			// Upper nibble shall never be F.
			if ( upperNibble == Image_MAXIMUM_RGB_NIBBLE ) {
				upperNibble = Image_MAXIMUM_ROUNDABLE;
			}

			// Now, shift new Sega value nibble by ( 4 * ( ( channel % 3 ) + 1 ) ) - 4
			// then bitwise "OR" it onto channelCol
			// On the first run, this applies R to the Sega R component, G on the second one, B on the third
			newComponentCol = ( u16 ) upperNibble;
			segaPalette[ triplet / 3 ] |= newComponentCol << ( ( 4 * ( ( channel % 3 ) + 1 ) ) - 4 );
		}
	}

	return segaPalette;
}

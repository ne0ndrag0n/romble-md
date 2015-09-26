#include <image.h>
#include <sizedarray.h>
#include <types.h>
#include <romble.h>
#include <stdlib.h>
#include <res/globals.h>

Image_vtable Image_table = {
	Image_dtor,
	Image_loadData,
	Image_buildPalette,
	Image_getVDPTiles
};

void Image_ctor( Image* this ) {
	this->imageData = NULL;
	this->vdpTiles = NULL;

	this->width = 0;
	this->height = 0;

	this->functions = &Image_table;
	FUNCTIONS( Image, Image, this )->buildPalette( this, NULL );
}

void Image_dtor( Image* this ) {
	if( this->imageData != NULL ) {
		free( this->imageData->items );
	}
	Romble_secureFree( ( void* ) &( this->imageData ) );

	if( this->vdpTiles != NULL ) {
		free( this->vdpTiles->items );
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

/**
 * Abstract, extend in your own class to construct the Sega-compatible
 * VDP palette of 16 colours and set both this->palette and this->nativePalette
 */
void Image_buildPalette( Image* this, Image_nativePalette nativePalette ) {
	this->nativePalette = nativePalette;
	this->palette = nativePalette;
}

SizedArray* Image_getVDPTiles( Image* this, bool keep ) {

	if( this->vdpTiles != NULL ) {
		return this->vdpTiles;
	}

	SizedArray* vdpTiles = calloc( 1, sizeof( SizedArray ) );

	if( keep == TRUE ) {
		this->vdpTiles = vdpTiles;
	}

	return vdpTiles;
}

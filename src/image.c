#include <image.h>
#include <sizedarray.h>
#include <types.h>
#include <romble.h>
#include <stdlib.h>
#include <res/globals.h>

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

#include <image.h>
#include <sizedarray.h>
#include <types.h>
#include <romble.h>
#include <stdlib.h>

void Image_ctor( Image* this ) {
	this->imageData = NULL;
	this->vdpTiles = NULL;
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

SizedArray Image_getVDPTiles( Image* this ) {
	SizedArray vdpTiles = { NULL, 0 };

	return vdpTiles;
}

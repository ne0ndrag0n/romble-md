#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>
#include <romble.h>
#include <lang.h>
#include <types.h>
#include <romble.h>
#include <res/globals.h>
#include <vdp.h>
#include <vdp_spr.h>
#include <vdp_pal.h>
#include <utility.h>

const u32 HaloTiles[ 24 ] = {
	0x00000000, 0x00000000, 0x00333333, 0x003BBBBB, 0x003BBBBB, 0x003BB333, 0x003BB300, 0x003BB300, 	//  Tile: 0
	0x00000000, 0x00000000, 0x33333333, 0xBBBBBBBB, 0xBBBBBBBB, 0x33333333, 0x00000000, 0x00000000, 	//  Tile: 1
	0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300 	//  Tile: 2
};

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY ) {
	this->registeredElements = calloc( registerableY, sizeof( SelectableElement** ) );

	Romble_assert( this->registeredElements != NULL, EXCEPTION_OUT_OF_MEMORY );

	size_t i;
	for( i = 0; i < registerableY; i++ ) {
		this->registeredElements[ i ] = calloc( registerableX, sizeof( SelectableElement* ) );
		Romble_assert( this->registeredElements[ i ] != NULL, EXCEPTION_OUT_OF_MEMORY );
	}

	this->registerableX = registerableX;
	this->registerableY = registerableY;

	this->haloTilesIndex = Romble_loadTiles( HaloTiles, 3 );
	this->currentElement = NULL;
}

void JoyManager_dtor( JoyManager* this ) {
	free( this->currentElement );
	
	// todo: clear the subelements in this->registeredElements!

	free( this->registeredElements );
}

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h ) {
	this->registeredElements[ x ][ y ] = malloc( sizeof( SelectableElement ) );

	this->registeredElements[ x ][ y ]->x = x;
	this->registeredElements[ x ][ y ]->y = y;
	this->registeredElements[ x ][ y ]->w = w;
	this->registeredElements[ x ][ y ]->h = h;
}

void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y ) {
	free( this->registeredElements[ x ][ y ] );
}

void JoyManager_displayCursor( JoyManager* this, bool show ) {
	// If show is true, call the renderSprites function
	if( show == TRUE ) {
		JoyManager_renderSprites( this );
	} else {

	}
}


void JoyManager_renderSprites( JoyManager* this ) {
	// Render a square of sprites around this->currentElement
	VDP_resetSprites();
	s16 absX = this->currentElement->x * 8;
	s16 absY = this->currentElement->y * 8;

	// setup corners
	this->corners[ 0 ].posx = absX - 4;
	this->corners[ 0 ].posy = absY - 4;
	this->corners[ 0 ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, FALSE, FALSE, this->haloTilesIndex );
	this->corners[ 0 ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ 0 ].link = 1;
	VDP_setSpriteP( 0, &( this->corners[ 0 ] ) );

	this->corners[ 1 ].posx = absX + ( this->currentElement->w * 8 ) - 4;
	this->corners[ 1 ].posy = absY - 4;
	this->corners[ 1 ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, FALSE, TRUE, this->haloTilesIndex );
	this->corners[ 1 ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ 1 ].link = 2;
	VDP_setSpriteP( 1, &( this->corners[ 1 ] ) );

	this->corners[ 2 ].posx = absX + ( this->currentElement->w * 8 ) - 4;
	this->corners[ 2 ].posy = absY + ( this->currentElement->h * 8 ) - 4;
	this->corners[ 2 ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, TRUE, TRUE, this->haloTilesIndex );
	this->corners[ 2 ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ 2 ].link = 3;
	VDP_setSpriteP( 2, &( this->corners[ 2 ] ) );

	this->corners[ 3 ].posx = absX - 4;
	this->corners[ 3 ].posy = absY + ( this->currentElement->h * 8 ) - 4;
	this->corners[ 3 ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, TRUE, FALSE, this->haloTilesIndex );
	this->corners[ 3 ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ 3 ].link = 0;
	VDP_setSpriteP( 3, &( this->corners[ 3 ] ) );

	VDP_updateSprites();
}

SelectableElementList JoyManager_retrieveSelectableElements( JoyManager* this, ElementRetrieval method ) {
	// Use ElementRetrieval method to determine what pointers to return
	size_t y, x, stopY, stopX;
	SelectableElementList result = { NULL, 0 };

	switch( method ) {

		case GREATER_THAN_X:
			// For each y, return non-null pointers for this->currentElement->x + 1 to this->registerableX - 1
			y = 0; 
			stopY = this->registerableY;

			x = this->currentElement->x + 1; 
			stopX = this->registerableX;
			break;
		case LESS_THAN_X:
			// For each y, return non-null pointers for 0 to this->currentElement->x
			y = 0;
			stopY = this->registerableY;

			x = 0;
			stopX = this->currentElement->x;
			break;
		case GREATER_THAN_Y:
			// For each x, return non-null pointers for this->currentElement->y + 1 to this->registerableY - 1
			y = this->currentElement->y + 1;
			stopY = this->registerableY;

			x = 0;
			stopX = this->registerableX;
			break;
		case LESS_THAN_Y:
			// For each x, return non-null pointers for 0 to this->currentElement->y - 1
			y = 0;
			stopY = this->currentElement->y;

			x = 0;
			stopX = this->registerableX;
			break;
	}

	for( ; y != stopY; y++ ) {
		for( ; x != stopX; x++ ) {
			if( this->registeredElements[ y ][ x ] != NULL ) {
				SelectableElement** resized = realloc( result.list, sizeof( SelectableElement* ) * ++result.length );
				Romble_assert( resized != NULL, EXCEPTION_OUT_OF_MEMORY );

				result.list[ result.length - 1 ] = this->registeredElements[ y ][ x ];
			}
		}
	}

	return result;
}

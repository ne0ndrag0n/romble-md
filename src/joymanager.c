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
#include <joy.h>

JoyManager* joyManager;

const u32 HaloTiles[ 24 ] = {
	0x00000000, 0x00000000, 0x00333333, 0x003BBBBB, 0x003BBBBB, 0x003BB333, 0x003BB300, 0x003BB300, 	//  Tile: 0
	0x00000000, 0x00000000, 0x33333333, 0xBBBBBBBB, 0xBBBBBBBB, 0x33333333, 0x00000000, 0x00000000, 	//  Tile: 1
	0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300, 0x003BB300 	//  Tile: 2
};

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY ) {
	this->registeredElements = calloc( registerableY, sizeof( SelectableElement** ) );

	Romble_assert( this->registeredElements != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );

	size_t i;
	for( i = 0; i < registerableY; i++ ) {
		this->registeredElements[ i ] = calloc( registerableX, sizeof( SelectableElement* ) );
		Romble_assert( this->registeredElements[ i ] != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	}

	this->registerableX = registerableX;
	this->registerableY = registerableY;

	this->haloTilesIndex = Romble_loadTiles( HaloTiles, 3 );
	this->currentElement = NULL;

	for( i = 0; i != 4; i++ ) {
		this->corners[ i ] = ( SpriteDef ){ 0, 0, 0, 0, 0 };
	}

	JOY_init();
	JOY_setEventHandler( &JoyManager_handlerBridge );

	VDP_resetSprites();
}

void JoyManager_dtor( JoyManager* this ) {
	free( this->currentElement );
	this->currentElement = NULL;

	// todo: clear the subelements in this->registeredElements!

	free( this->registeredElements );
	this->registeredElements = NULL;
}

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h, void* instance, JoyManager_Callback callback ) {
	this->registeredElements[ y ][ x ] = malloc( sizeof( SelectableElement ) );

	this->registeredElements[ y ][ x ]->x = x;
	this->registeredElements[ y ][ x ]->y = y;
	this->registeredElements[ y ][ x ]->w = w;
	this->registeredElements[ y ][ x ]->h = h;
	this->registeredElements[ y ][ x ]->instance = instance;
	this->registeredElements[ y ][ x ]->callback = callback;
}

void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y ) {
	free( this->registeredElements[ y ][ x ] );
	this->registeredElements[ y ][ x ] = NULL;
}

void JoyManager_displayCursor( JoyManager* this, bool show ) {
	// If show is true, call the renderSprites function
	if( show == TRUE ) {
		JoyManager_renderSprites( this );
	}
}

void JoyManager_renderSprites( JoyManager* this ) {
	// Render a square of sprites around this->currentElement
	JoyManager_positionSprites( this, this->currentElement );
	VDP_setSpritesDirect( 0, this->corners, 4 );
}

void JoyManager_positionSprites( JoyManager* this, SelectableElement* targetElement ) {
	s16 absX = targetElement->x * 8;
	s16 absY = targetElement->y * 8;

	// setup corners
	this->corners[ SELECTOR_UPPER_LEFT ].posx = absX - 4;
	this->corners[ SELECTOR_UPPER_LEFT ].posy = absY - 4;
	this->corners[ SELECTOR_UPPER_LEFT ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, FALSE, FALSE, this->haloTilesIndex );
	this->corners[ SELECTOR_UPPER_LEFT ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ SELECTOR_UPPER_LEFT ].link = SELECTOR_UPPER_RIGHT;

	this->corners[ SELECTOR_UPPER_RIGHT ].posx = absX + ( targetElement->w * 8 ) - 4;
	this->corners[ SELECTOR_UPPER_RIGHT ].posy = absY - 4;
	this->corners[ SELECTOR_UPPER_RIGHT ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, FALSE, TRUE, this->haloTilesIndex );
	this->corners[ SELECTOR_UPPER_RIGHT ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ SELECTOR_UPPER_RIGHT ].link = SELECTOR_LOWER_RIGHT;

	this->corners[ SELECTOR_LOWER_RIGHT ].posx = absX + ( targetElement->w * 8 ) - 4;
	this->corners[ SELECTOR_LOWER_RIGHT ].posy = absY + ( targetElement->h * 8 ) - 4;
	this->corners[ SELECTOR_LOWER_RIGHT ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, TRUE, TRUE, this->haloTilesIndex );
	this->corners[ SELECTOR_LOWER_RIGHT ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ SELECTOR_LOWER_RIGHT ].link = SELECTOR_LOWER_LEFT;

	this->corners[ SELECTOR_LOWER_LEFT ].posx = absX - 4;
	this->corners[ SELECTOR_LOWER_LEFT ].posy = absY + ( targetElement->h * 8 ) - 4;
	this->corners[ SELECTOR_LOWER_LEFT ].tile_attr = TILE_ATTR_FULL( PAL0, PRIORITY_HIGH, TRUE, FALSE, this->haloTilesIndex );
	this->corners[ SELECTOR_LOWER_LEFT ].size = SPRITE_SIZE( 1, 1 );
	this->corners[ SELECTOR_LOWER_LEFT ].link = SPRITE_LIST_END;
}

void JoyManager_moveToNearest( JoyManager* this, SelectableElementList* neighbourhood ) {
	SelectableElement* nearest = NULL;
	s16 currentDistance = 0;
	s16 lastKnownDistance = MAXIMUM_DISTANCE;
	size_t i;

	if( neighbourhood->length > 0 ) {
		for( i = 0; i != neighbourhood->length; i++ ) {
			currentDistance = DISTANCE(
				this->currentElement->x, neighbourhood->list[ i ]->x,
				this->currentElement->y, neighbourhood->list[ i ]->y
			);
			if( currentDistance < lastKnownDistance ) {
				nearest = neighbourhood->list[ i ];
				lastKnownDistance = currentDistance;
			}
		}

		// There should be at least one element found here, if not, assertion failed
		Romble_assert( nearest != NULL, FILE_LINE( EXCEPTION_NULL_POINTER ) );

		JoyManager_animateCursorMovement( this, nearest );

		free( neighbourhood->list );
		neighbourhood->list = NULL;
	} else {
		// Can't move anywhere - call on sound subsystem to play a "donk" noise
	}
}

void JoyManager_animateCursorMovement( JoyManager* this, SelectableElement* newLocation ) {
	SelectableElement currentPosition = { 0 };

	s16 startX = this->currentElement->x * 8;
	s16 endX   = newLocation->x * 8;

	s16 startY = this->currentElement->y * 8;
	s16 endY   = newLocation->y * 8;

	s16 startW = this->currentElement->w * 8;
	s16 endW   = newLocation->w * 8;

	s16 startH = this->currentElement->h * 8;
	s16 endH   = newLocation->h * 8;

	size_t i;
	for( i = 0; i <= 100; i+=2 ) {
		currentPosition.x = Utility_lerp( endX, startX, i );
		currentPosition.y = Utility_lerp( endY, startY, i );
		currentPosition.w = Utility_lerp( endW, startW, i );
		currentPosition.h = Utility_lerp( endH, startH, i );

		this->corners[ SELECTOR_UPPER_LEFT ].posx = currentPosition.x - 4;
		this->corners[ SELECTOR_UPPER_LEFT ].posy = currentPosition.y - 4;

		this->corners[ SELECTOR_UPPER_RIGHT ].posx = currentPosition.x + currentPosition.w - 4;
		this->corners[ SELECTOR_UPPER_RIGHT ].posy = currentPosition.y - 4;

		this->corners[ SELECTOR_LOWER_RIGHT ].posx = currentPosition.x + currentPosition.w - 4;
		this->corners[ SELECTOR_LOWER_RIGHT ].posy = currentPosition.y + currentPosition.h - 4;

		this->corners[ SELECTOR_LOWER_LEFT ].posx = currentPosition.x - 4;
		this->corners[ SELECTOR_LOWER_LEFT ].posy = currentPosition.y + currentPosition.h - 4;

		VDP_waitVSync();
		VDP_setSpritesDirect( 0, this->corners, 4 );
	}

	this->currentElement = newLocation;
}

void JoyManager_setDefaultCurrentElement( JoyManager* this ) {
	size_t y, x;
	SelectableElement* newDefault = NULL;

	for( y = 0; y != this->registerableY; y++ ) {
		for( x = 0; x != this->registerableX; x++ ) {
			if( this->registeredElements[ y ][ x ] != NULL ) {
				newDefault = this->registeredElements[ y ][ x ];
				goto exitLoop;
			}
		}
	}

exitLoop:
	this->currentElement = newDefault;
}

SelectableElementList JoyManager_retrieveSelectableElements( JoyManager* this, ElementRetrieval method ) {
	// Use ElementRetrieval method to determine what pointers to return
	s16 y = 0, x = 0, stopY = 0, stopX = 0, xIndex = 0;
	SelectableElementList result = { NULL, 0 };

	if( this->currentElement != NULL ) {
		switch( method ) {

			case ElementRetrieval_GREATER_THAN_X:
				// For each y, return non-null pointers for this->currentElement->x + 1 to this->registerableX - 1
				y = 0;
				stopY = this->registerableY;

				x = this->currentElement->x + 1;
				stopX = this->registerableX;
				break;
			case ElementRetrieval_LESS_THAN_X:
				// For each y, return non-null pointers for 0 to this->currentElement->x
				y = 0;
				stopY = this->registerableY;

				x = 0;
				stopX = this->currentElement->x;
				break;
			case ElementRetrieval_GREATER_THAN_Y:
				// For each x, return non-null pointers for this->currentElement->y + 1 to this->registerableY - 1
				y = this->currentElement->y + 1;
				stopY = this->registerableY;

				x = 0;
				stopX = this->registerableX;
				break;
			case ElementRetrieval_LESS_THAN_Y:
				// For each x, return non-null pointers for 0 to this->currentElement->y - 1
				y = 0;
				stopY = this->currentElement->y;

				x = 0;
				stopX = this->registerableX;
				break;
		}


		for( ; y != stopY; y++ ) {
			for( xIndex = x; xIndex != stopX; xIndex++ ) {
				if( this->registeredElements[ y ][ xIndex ] != NULL ) {
					SelectableElement** resized = realloc( result.list, sizeof( SelectableElement* ) * ++result.length );
					Romble_assert( resized != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );

					result.list = resized;

					result.list[ result.length - 1 ] = this->registeredElements[ y ][ xIndex ];
				}
			}
		}
	}

	return result;
}

void JoyManager_handleInput( JoyManager* this, u16 joy, u16 changed, u16 state ) {
	switch( joy ) {
		case JOY_1:
			// we can probably do better with a sparse array of function pointers

			if( state & BUTTON_UP ) {
				SelectableElementList usableElements = JoyManager_retrieveSelectableElements( this, ElementRetrieval_LESS_THAN_Y );
				JoyManager_moveToNearest( this, &usableElements );
				break;
			}

			if( state & BUTTON_DOWN ) {
				SelectableElementList usableElements = JoyManager_retrieveSelectableElements( this, ElementRetrieval_GREATER_THAN_Y );
				JoyManager_moveToNearest( this, &usableElements );
				break;
			}

			if( state & BUTTON_LEFT ) {
				SelectableElementList usableElements = JoyManager_retrieveSelectableElements( this, ElementRetrieval_LESS_THAN_X );
				JoyManager_moveToNearest( this, &usableElements );
				break;
			}

			if( state & BUTTON_RIGHT ) {
				SelectableElementList usableElements = JoyManager_retrieveSelectableElements( this, ElementRetrieval_GREATER_THAN_X );
				JoyManager_moveToNearest( this, &usableElements );
				break;
			}

			// buttons
			if( state & ( BUTTON_A | BUTTON_B | BUTTON_C ) ) {
				if( this->currentElement->callback != NULL ) {
					this->currentElement->callback( this->currentElement->instance, state );
				}
			}
			break;
		default:
			break;
	}
}

/**
 * Bridge the expected input from the SGDK joy driver to the instance of the global joyManager
 */
void JoyManager_handlerBridge( u16 joy, u16 changed, u16 state ) {
	JoyManager_handleInput( joyManager, joy, changed, state );
}

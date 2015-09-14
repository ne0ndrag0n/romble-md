#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>
#include <romble.h>
#include <lang.h>
#include <types.h>

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
}

void JoyManager_dtor( JoyManager* this ) {
	free( this->currentElement );
	free( this->registeredElements );
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

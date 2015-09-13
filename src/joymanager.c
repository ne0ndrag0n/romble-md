#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>
#include <romble.h>
#include <lang.h>

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
	size_t y, x;
	SelectableElementList result = { NULL, 0 };

	switch( method ) {
		case GREATER_THAN_X:
			// For each y, return non-null pointers for this->currentElement->x + 1 to this->registerableX - 1
			for( y = 0; y != this->registerableY; y++ ) {
				for( x = this->currentElement->x + 1; x != this->registerableX; x++ ) {
					if( this->registeredElements[ y ][ x ] != NULL ) {
						SelectableElement** resized = realloc( result.list, sizeof( SelectableElement* ) * ++result.length );
						Romble_assert( resized != NULL, EXCEPTION_OUT_OF_MEMORY );

						result.list[ result.length - 1 ] = this->registeredElements[ y ][ x ];
					}
				}
			}
			break;
	}


	return result;
}

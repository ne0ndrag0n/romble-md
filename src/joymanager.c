#include <joymanager.h>
#include <stdlib.h>
#include <stddef.h>

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY ) {
	this->registeredElements = calloc( registerableY, sizeof( SelectableElement** ) );

	size_t i;
	for( i = 0; i < registerableY; i++ ) {
		this->registeredElements[ i ] = calloc( registerableX, sizeof( SelectableElement* ) );
	}
}

void JoyManager_dtor( JoyManager* this ) {
	free( this->currentElement );
	free( this->registeredElements );
}

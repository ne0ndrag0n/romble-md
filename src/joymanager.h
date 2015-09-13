#ifndef JOYMANAGER
#define JOYMANAGER

#include <types.h>

struct JoyManager;

typedef struct SelectableElement {
	s16 x;
	s16 y;
	s16 w;
	s16 h;
} SelectableElement;

typedef struct JoyManager {
	SelectableElement* currentElement;

	SelectableElement*** registeredElements;
} JoyManager;

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY );
void JoyManager_dtor( JoyManager* this );

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h );
void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y );

#endif

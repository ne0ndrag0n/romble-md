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

typedef enum {
	GREATER_THAN_X,
	LESS_THAN_X,
	GREATER_THAN_Y,
	LESS_THAN_Y
} ElementRetrieval;

typedef struct SelectableElementList {
	SelectableElement** list;
	u8 length;
} SelectableElementList;

typedef struct JoyManager {
	SelectableElement* currentElement;

	u8 registerableX;
	u8 registerableY;
	SelectableElement*** registeredElements;
} JoyManager;

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY );
void JoyManager_dtor( JoyManager* this );

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h );
void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y );

SelectableElementList JoyManager_retrieveSelectableElements( JoyManager* this, ElementRetrieval method );

#endif

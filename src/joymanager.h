#ifndef JOYMANAGER
#define JOYMANAGER

#include <types.h>
#include <res/globals.h>
#include <vdp_spr.h>

extern const u32 HaloTiles[ 24 ];

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
	SpriteDef corners[ 4 ];
	u8 registerableX;
	u8 registerableY;
	u16 haloTilesIndex;
	SelectableElement*** registeredElements;
} JoyManager;

extern JoyManager* joyManager;

void JoyManager_ctor( JoyManager* this, u8 registerableX, u8 registerableY );
void JoyManager_dtor( JoyManager* this );

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h );
void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y );

void JoyManager_displayCursor( JoyManager* this, bool show );
void JoyManager_renderSprites( JoyManager* this );

SelectableElementList JoyManager_retrieveSelectableElements( JoyManager* this, ElementRetrieval method );

void JoyManager_handlerBridge( u16 joy, u16 changed, u16 state );

#endif

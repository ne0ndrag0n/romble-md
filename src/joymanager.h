#ifndef JOYMANAGER
#define JOYMANAGER

#include <types.h>
#include <res/globals.h>
#include <vdp_spr.h>

// bloody hell, it's like lisp
#define DISTANCE( x2, x1, y2, y1 ) ( ( ( x2 ) - ( x1 ) )*( ( x2 ) - ( x1 ) ) ) + ( ( ( y2 ) - ( y1 ) )*( ( y2 ) - ( y1 ) ) )

#define MAXIMUM_DISTANCE		32767
#define SELECTOR_UPPER_LEFT 	0
#define SELECTOR_UPPER_RIGHT	1
#define	SELECTOR_LOWER_RIGHT	2
#define SELECTOR_LOWER_LEFT		3

#define	SPRITE_LIST_END			0

extern const u32 HaloTiles[ 24 ];
typedef u16 JoyManager_Button;
typedef void ( *JoyManager_Callback )( void*, JoyManager_Button );

struct JoyManager;

typedef struct SelectableElement {
	s16 x;
	s16 y;
	s16 w;
	s16 h;
	JoyManager_Callback callback;
} SelectableElement;

typedef enum {
	ElementRetrieval_GREATER_THAN_X,
	ElementRetrieval_LESS_THAN_X,
	ElementRetrieval_GREATER_THAN_Y,
	ElementRetrieval_LESS_THAN_Y
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

void JoyManager_registerElement( JoyManager* this, s16 x, s16 y, s16 w, s16 h, JoyManager_Callback callback );
void JoyManager_unregisterElement( JoyManager* this, s16 x, s16 y );

void JoyManager_displayCursor( JoyManager* this, bool show );
void JoyManager_positionSprites( JoyManager* this );
void JoyManager_renderSprites( JoyManager* this );
void JoyManager_moveToNearest( JoyManager* this, SelectableElementList* neighbourhood );

SelectableElementList JoyManager_retrieveSelectableElements( JoyManager* this, ElementRetrieval method );

void JoyManager_handlerBridge( u16 joy, u16 changed, u16 state );
void JoyManager_handleInput( JoyManager* this, u16 joy, u16 changed, u16 state );

#endif

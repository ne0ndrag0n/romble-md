#include <supervisor.h>
#include <types.h>
#include <res/globals.h>
#include <baseview.h>
#include <vdpmanager.h>
#include <eventmanager.h>
#include <lang.h>
#include <tags.h>
#include <log.h>
#include <romble.h>
// state views
#include <testview.h>

Supervisor_vtable Supervisor_table = {
	BaseView_dtor,
	Supervisor_render,
	BaseView_position,
	BaseView_renderChildren,
	BaseView_addChildView,
	BaseView_placeTile,
	BaseView_placeTileSeries,

	BaseView_listenToView,
	BaseView_stopListeningView,
	BaseView_trigger,

	BaseView_getChildById,
	BaseView_getChildByTag,
	BaseView_removeChild,

	BaseView_checkTileBoundary,

	Supervisor_changeState,
	Supervisor_clearPlane
};

void Supervisor_ctor( Supervisor* this ) {
	// Span the supervisor view the full length of the Sega plane field
	BaseView_ctor( CLASS( BaseView, this ), 0, 0, VDPManager_PLANE_MAX_X, VDPManager_PLANE_MAX_Y );

	// Setup vtable
	CLASS( BaseView, this )->functions = &Supervisor_table;

	// The first state is set as Supervisor_State_DEFAULT
	this->currentState = Supervisor_State_DEFAULT;
	// Has no view
	this->stateView = NULL;

	// Register the event on the global event manager (any view may trigger a state change this way)
	EventManager_registerEvent( eventManager, this, EVENT_NAVIGATE, Supervisor_onChangeState );
}

void Supervisor_render( Supervisor* this ) {
	FUNCTIONS( Supervisor, BaseView, this )->position( CLASS( BaseView, this ) );

	FUNCTIONS( Supervisor, BaseView, this )->clearPlane( this );

	FUNCTIONS( Supervisor, BaseView, this )->renderChildren( CLASS( BaseView, this ) );
}

void Supervisor_clearPlane( Supervisor* this ) {
	// Get system palette
	VDPManager_PaletteIndex systemPalette = VDPManager_getPaletteByTag( vdpManager, PAL_SYSTEM );
	if( systemPalette == VDPManager_Palette_INVALID ) {
		// Need to load the system palette - Ordinarily, this should never be done.
		Log_message( Log_Level_WARNING, FILE_LINE(), "system palette wasn't loaded when initialising Supervisor" );
		systemPalette = VDPManager_loadPalette( vdpManager, StandardColours, PAL_SYSTEM );
	}

	FUNCTIONS( Supervisor, BaseView, this )->placeTileSeries(
		CLASS( BaseView, this ),
		VDPManager_LAYER_BASE,
		0,
		0,
		CLASS( BaseView, this )->width,
		CLASS( BaseView, this )->height,
		systemPalette,
		0x0000,
		FALSE
	);
	FUNCTIONS( Supervisor, BaseView, this )->placeTileSeries(
		CLASS( BaseView, this ),
		VDPManager_LAYER_TRANSPARENT,
		0,
		0,
		CLASS( BaseView, this )->width,
		CLASS( BaseView, this )->height,
		systemPalette,
		0x0000,
		FALSE
	);
}

void Supervisor_changeState( Supervisor* this, Supervisor_State state ) {
	// Depending on the given Supervisor_State, setup a view that displays as a child of the supervisor

	// Destroy + remove the stateView (added as this->stateView). This will re-render the supervisor,
	// clearing both VDP planes.
	if( this->stateView != NULL ) {
		FUNCTIONS( Supervisor, BaseView, this )->removeChild( CLASS( BaseView, this ), this->stateView, FALSE );
		this->stateView = NULL;
	} else {
		// Make sure the view is positioned and cleared without the overhead of removeChild
		FUNCTIONS( Supervisor, BaseView, this )->render( this );
	}

	// Now, select a new state, and insert the view
	switch( state ) {
		case Supervisor_State_WELCOME:
		case Supervisor_State_LOGIN:
		case Supervisor_State_BETATEST: {
			Log_message( Log_Level_DEBUG, FILE_LINE(), "Setting up state Supervisor_State_BETATEST" );
			TestView* view = NULL;
			NEW_OBJECT( TestView, view, 2, 2, 35, 20 );
			FUNCTIONS( Supervisor, BaseView, this )->addChildView( CLASS( BaseView, this ), CLASS( BaseView, view ) );

			this->stateView = ( BaseView* ) view;
			break;
		}
		case Supervisor_State_DEFAULT:
		default:
			Log_message( Log_Level_WARNING, FILE_LINE(), "Unable to make state transition; Supervisor did not receive a valid state." );
			break;
	}

	// Re-render the supervisor
	FUNCTIONS( Supervisor, BaseView, this )->render( this );
}

void Supervisor_onChangeState( void* instance, void* payload ) {
	Supervisor* this = instance;
	Supervisor_State state = *( ( Supervisor_State* ) payload );

	FUNCTIONS( Supervisor, BaseView, this )->changeState( this, state );
}

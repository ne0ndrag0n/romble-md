#include <boxview.h>
#include <baseview.h>
#include <res/globals.h>
#include <stddef.h>
#include <vdpmanager.h>
#include <res/tags.h>
#include <log.h>

BoxView_vtable BoxView_table = {
	BaseView_dtor,
	BoxView_render,
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

	BaseView_checkTileBoundary
};

void BoxView_ctor( BoxView* this, u16 boxDrawingIndex, u16 fillTileIndex, s16 x, s16 y, s16 width, s16 height ) {
	BaseView_ctor( ( BaseView* ) this, x, y, width, height );

	this->fillTileIndex = fillTileIndex;
	this->boxDrawingIndex = boxDrawingIndex;

	this->super.functions = &BoxView_table;
}

void BoxView_render( BoxView* this ) {
	BaseView* super = ( BaseView* ) this;

	BaseView_render( super );

	void ( *placeTile )( struct BaseView*, s16, s16, s16, u8, u16, bool, bool ) = FUNCTIONS( BoxView, BaseView, this )->placeTile;

	// Get system palette
	VDPManager_PaletteIndex systemPalette = VDPManager_getPaletteByTag( vdpManager, PAL_SYSTEM );
	if( systemPalette == VDPManager_Palette_INVALID ) {
		// Need to load the system palette - Ordinarily, this should never be done.
		Log_message( Log_Level_WARNING, FILE_LINE(), "system palette wasn't loaded when initialising BoxView" );
		systemPalette = VDPManager_loadPalette( vdpManager, StandardColours, PAL_SYSTEM );
	}

	// Place corner tiles
	placeTile( super, VDPManager_LAYER_BASE, 0, 0, systemPalette, this->boxDrawingIndex, FALSE, FALSE );
	placeTile( super, VDPManager_LAYER_BASE, super->width - 1, 0, systemPalette, this->boxDrawingIndex, FALSE, TRUE );
	placeTile( super, VDPManager_LAYER_BASE, super->width - 1, super->height - 1, systemPalette, this->boxDrawingIndex, TRUE, TRUE );
	placeTile( super, VDPManager_LAYER_BASE, 0, super->height - 1, systemPalette, this->boxDrawingIndex, TRUE, FALSE );

	// Place tile runs
	size_t i,j;
	for ( i = 1; i != super->height - 1; i++ ) {
		for( j = 1; j != super->width - 1; j++ ) {
			placeTile( super, VDPManager_LAYER_BASE, j, i, systemPalette, this->fillTileIndex, FALSE, FALSE );
		}
	}


	for( i = 1; i != super->width - 1; i++ ) {
		// Horizontal runs
		placeTile( super, VDPManager_LAYER_BASE, i, 0, systemPalette, this->boxDrawingIndex + 1, FALSE, FALSE );
		placeTile( super, VDPManager_LAYER_BASE, i, super->height - 1, systemPalette, this->boxDrawingIndex + 1, TRUE, FALSE );
	}

	for( i = 1; i != super->height - 1; i++ ) {
		// Vertical runs
		placeTile( super, VDPManager_LAYER_BASE, 0, i, systemPalette, this->boxDrawingIndex + 2, FALSE, TRUE );
		placeTile( super, VDPManager_LAYER_BASE, super->width - 1, i, systemPalette, this->boxDrawingIndex + 2, FALSE, FALSE );
	}
}

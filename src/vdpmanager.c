#include <vdpmanager.h>
#include <types.h>
#include <stddef.h>
#include <res/globals.h>
#include <romble.h>

void VDPManager_ctor( VDPManager* this ) {

	for( size_t i = 0; i != 3; i++ ) {
		this->palettes[ i ] = NULL;
	}

	this->freeVDPSegments = NULL;
}

void VDPManager_dtor( VDPManager* this ) {
	// I don't think we'll ever need to do this?
}

VDPManager_TileIndex VDPManager_loadTiles( VDPManager* this, u32* tiles, u16 count ) {
	return 0;
}

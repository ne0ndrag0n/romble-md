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
	this->usedSegmentCount = NULL;
}

void VDPManager_dtor( VDPManager* this ) {
	// I don't think we'll ever need to do this?
}

VDPManager_TileIndex VDPManager_loadTiles( VDPManager* this, VDPManager_Tiles tiles, u16 count ) {
	if( this->freeVDPSegments == NULL ) {
		// If freeVDPSegments is null, nothing has yet been added to the VDP RAM starting at
		// VDPManager_TILE_USERINDEX
		this->freeVDPSegments = Romble_alloc_d( sizeof( VDPManager_VDPRamSegment ), TRUE, FILE_LINE() );
		this->usedSegmentCount = 1;

		this->freeVDPSegments[ 0 ].tileData = tiles;
		this->freeVDPSegments[ 0 ].index = VDPManager_TILE_USERINDEX;
		this->freeVDPSegments[ 0 ].length = count;
	} else {
		// We need to apply a first-fit algorithm that looks for the first continuous region of VDP RAM
		// that will fit the incoming data ("count" number of tiles). Take tiles two at a time, and use
		// the first tile's index + count to see if it matches the second tile's index. If it does not
		// match, there is a gap: check and see if the gap is equal to or lesser than count. If it is,
		// this is the first-fit gap we are looking for; if not, take another two tiles and apply the
		// same test.
		//
		// Once a gap has been found, we need to add a VDPRamSegment to this->freeVDPSegments, sort
		// this->freeVDPSegments to ensure this algorithm works, and then actually load the info.

	}
	return 0;
}

int VDPManager_qsortComparator( const void* firstItem, const void* secondItem ) {
	VDPManager_TileIndex firstIndex = ( ( VDPManager_VDPRamSegment* ) firstItem )->index;
	VDPManager_TileIndex secondIndex = ( ( VDPManager_VDPRamSegment* ) secondItem )->index;

	if( firstIndex == secondIndex ) {
		return 0;
	} else if ( firstIndex < secondIndex ) {
		return -1;
	} else {
		return 1;
	}
}

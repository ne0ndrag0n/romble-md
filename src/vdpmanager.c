#include <vdpmanager.h>
#include <types.h>
#include <stddef.h>
#include <stdlib.h>
#include <res/globals.h>
#include <romble.h>

void VDPManager_ctor( VDPManager* this ) {

	for( size_t i = 0; i != 3; i++ ) {
		this->palettes[ i ] = NULL;
	}

	this->freeVDPSegments = NULL;
	this->usedSegmentCount = 0;
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

		return this->freeVDPSegments[ 0 ].index;
	} else {
		// We need to apply a first-fit algorithm that looks for the first continuous region of VDP RAM
		// that will fit the incoming data ("count" number of tiles). Take tilesets two at a time, and use
		// the first tile's index + count to see if it matches the second tileset's index. If it does not
		// match, there is a gap: check and see if the gap is equal to or lesser than count. If it is,
		// this is the first-fit gap we are looking for; if not, take another two tilesets and apply the
		// same test.
		//
		// Once a gap has been found, we need to add a VDPRamSegment to this->freeVDPSegments, sort
		// this->freeVDPSegments to ensure this algorithm works, and then actually load the info.
		size_t i;
		size_t cap = this->usedSegmentCount % 2 ? this->usedSegmentCount - 1 : this->usedSegmentCount - 2;
		for( i = 0; i != cap; i++ ) {
			VDPManager_VDPRamSegment first = this->freeVDPSegments[ i ];
			VDPManager_VDPRamSegment second = this->freeVDPSegments[ i + 1 ];

			// Calculate the gap between the two elements
			u16 gapSize = ( second.index - first.index ) - first.length;
			if( count <= gapSize ) {
				// It fits in this gap - Add it to the array
				VDPManager_TileIndex tileIndex = first.index + first.length;

				this->usedSegmentCount++;
				this->freeVDPSegments = Romble_realloc_d( this->freeVDPSegments, sizeof( VDPManager_VDPRamSegment ) * this->usedSegmentCount, FILE_LINE() );
				this->freeVDPSegments[ this->usedSegmentCount - 1 ].tileData = tiles;
				this->freeVDPSegments[ this->usedSegmentCount - 1 ].index = tileIndex;
				this->freeVDPSegments[ this->usedSegmentCount - 1 ].length = count;

				// Keep sorted
				qsort( this->freeVDPSegments, this->usedSegmentCount, sizeof( VDPManager_VDPRamSegment ), VDPManager_qsortComparator );

				return tileIndex;
			}
		}

		// If we get here, there's been no existing gap large enough to fit in VDP RAM
		// Just toss it right after the last item
		VDPManager_VDPRamSegment lastSegment = this->freeVDPSegments[ this->usedSegmentCount - 1 ];
		VDPManager_TileIndex tileIndex = lastSegment.index + lastSegment.length;

		this->usedSegmentCount++;
		this->freeVDPSegments = Romble_realloc_d( this->freeVDPSegments, sizeof( VDPManager_VDPRamSegment ) * this->usedSegmentCount, FILE_LINE() );
		this->freeVDPSegments[ this->usedSegmentCount - 1 ].tileData = tiles;
		this->freeVDPSegments[ this->usedSegmentCount - 1 ].index = tileIndex;
		this->freeVDPSegments[ this->usedSegmentCount - 1 ].length = count;

		// This shouldn't require a resort as the index position is consistent

		return tileIndex;
	}
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

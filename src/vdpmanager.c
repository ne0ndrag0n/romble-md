#include <vdpmanager.h>
#include <types.h>
#include <stddef.h>
#include <stdlib.h>
#include <res/globals.h>
#include <romble.h>
#include <vdp_bg.h>

void VDPManager_ctor( VDPManager* this ) {

	for( size_t i = 0; i != 3; i++ ) {
		this->palettes[ i ] = NULL;
	}

	this->usedVDPSegments = NULL;
	this->usedSegmentCount = 0;
}

void VDPManager_dtor( VDPManager* this ) {
	// I don't think we'll ever need to do this?
}

VDPManager_TileIndex VDPManager_loadTiles( VDPManager* this, VDPManager_Tiles tiles, u16 count, VDPManager_Tag tag ) {
	if( this->usedVDPSegments == NULL ) {
		// If usedVDPSegments is null, nothing has yet been added to the VDP RAM starting at
		// VDPManager_TILE_USERINDEX

		VDPManager_pushSegment( this, tiles, VDPManager_TILE_USERINDEX, count, tag );
		VDP_loadTileData( tiles, VDPManager_TILE_USERINDEX, count, TRUE );

		return this->usedVDPSegments[ 0 ].index;
	} else {
		// We need to apply a first-fit algorithm that looks for the first continuous region of VDP RAM
		// that will fit the incoming data ("count" number of tiles). Take tilesets two at a time, and use
		// the first tile's index + count to see if it matches the second tileset's index. If it does not
		// match, there is a gap: check and see if the gap is equal to or lesser than count. If it is,
		// this is the first-fit gap we are looking for; if not, take another two tilesets and apply the
		// same test.
		//
		// Once a gap has been found, we need to add a VDPRamSegment to this->usedVDPSegments, sort
		// this->usedVDPSegments to ensure this algorithm works, and then actually load the info.
		size_t i;
		size_t cap = this->usedSegmentCount % 2 ? this->usedSegmentCount - 1 : this->usedSegmentCount - 2;
		for( i = 0; i != cap; i++ ) {
			VDPManager_VDPRamSegment first = this->usedVDPSegments[ i ];
			VDPManager_VDPRamSegment second = this->usedVDPSegments[ i + 1 ];

			// Calculate the gap between the two elements
			u16 gapSize = ( second.index - first.index ) - first.length;
			if( count <= gapSize ) {
				// It fits in this gap - Add it to the array
				VDPManager_TileIndex tileIndex = first.index + first.length;

				VDPManager_pushSegment( this, tiles, tileIndex, count, tag );
				VDP_loadTileData( tiles, tileIndex, count, TRUE );

				// Keep sorted
				qsort( this->usedVDPSegments, this->usedSegmentCount, sizeof( VDPManager_VDPRamSegment ), VDPManager_qsortComparator );

				return tileIndex;
			}
		}

		// If we get here, there's been no existing gap large enough to fit in VDP RAM
		// Just toss it right after the last item
		VDPManager_VDPRamSegment lastSegment = this->usedVDPSegments[ this->usedSegmentCount - 1 ];
		VDPManager_TileIndex tileIndex = lastSegment.index + lastSegment.length;

		VDPManager_pushSegment( this, tiles, tileIndex, count, tag );
		VDP_loadTileData( tiles, tileIndex, count, TRUE );

		// This shouldn't require a resort as the index position is consistent
		return tileIndex;
	}
}

void VDPManager_pushSegment( VDPManager* this, VDPManager_Tiles tiles, VDPManager_TileIndex index, u16 length, VDPManager_Tag tag ) {
	this->usedSegmentCount++;

	this->usedVDPSegments = Romble_realloc_d( this->usedVDPSegments, sizeof( VDPManager_VDPRamSegment ) * this->usedSegmentCount, FILE_LINE() );

	this->usedVDPSegments[ this->usedSegmentCount - 1 ].tileData = tiles;
	this->usedVDPSegments[ this->usedSegmentCount - 1 ].index = index;
	this->usedVDPSegments[ this->usedSegmentCount - 1 ].length = length;
	this->usedVDPSegments[ this->usedSegmentCount - 1 ].tag = tag;
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

VDPManager_TileIndex VDPManager_getTilesByTag( VDPManager* this, VDPManager_Tag tag ) {
	// The invalid value will be VDPManager_INDEX_NULL

	for( size_t i = 0; i != this->usedSegmentCount; i++ ) {
		VDPManager_VDPRamSegment current = this->usedVDPSegments[ i ];

		if( current.tag == tag ) {
			return current.index;
		}
	}

	return VDPManager_INDEX_NULL;
}

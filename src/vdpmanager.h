#ifndef PAL_MANAGER
#define PAL_MANAGER

#include <types.h>
#include <res/globals.h>
#include <vdp.h>

#define		VDPManager_TILE_USERINDEX	TILE_USERINDEX
#define		VDPManager_TILE_MAX			0x01FF

typedef u16 VDPManager_TileIndex;

typedef struct VDPManager_VDPRamSegment {
	u32* tileData;
	u16 index;
	u16 length;
} VDPManager_VDPRamSegment;

typedef struct VDPManager {
	u16* palettes[ 3 ];
	VDPManager_VDPRamSegment* freeVDPSegments;
} VDPManager;

void VDPManager_ctor( VDPManager* this );
void VDPManager_dtor( VDPManager* this );

VDPManager_TileIndex VDPManager_loadTiles( VDPManager* this, u32* tiles, u16 count );

#endif

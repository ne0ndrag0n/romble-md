#ifndef PAL_MANAGER
#define PAL_MANAGER

#include <types.h>
#include <res/globals.h>
#include <vdp.h>
#include <linkedlist.h>

typedef u16 VDPManager_TileIndex;

typedef struct VDPManager {
	u16* palettes[ 3 ];
	LinkedListNode* freeVDPSegments;
} VDPManager;

void VDPManager_ctor( VDPManager* this );
void VDPManager_dtor( VDPManager* this );

VDPManager_TileIndex VDPManager_loadTiles( VDPManager* this, u32* tiles, u16 count );


#endif

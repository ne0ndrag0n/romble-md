#include <test/vdp_manager.h>
#include <test/test_framework.h>
#include <types.h>
#include <res/globals.h>
#include <res/tags.h>
#include <stdlib.h>
#include <stdio.h>
#include <vdpmanager.h>
#include <vdp.h>
#include <globals.h>

// use BoxDrawingCharacters

const TestFramework_TestCaseDefinition VDPManagerTests[] = {
	{
		"Should properly initialize a VDPManager",
		VDPManagerTests_verifyCtor
	},
	{
		"Should properly load one set of tiles into the VDPManager starting from NULL",
		VDPManagerTests_verifyLoadTilesFromNull
	},
	{
		"Should properly add a second VDPRamSegment of tiles at first.index + first.length",
		VDPManagerTests_verifyLoadSecondTile
	},
	{
		"Should properly add three items without a gap",
		VDPManagerTests_verifyThirdTileNoGap
	},
	{
		"Shold properly add an item inside a gap",
		VDPManagerTests_verifyThirdTileGap
	}
};

const u32 VDPManagerTests_TestTwoTiles[ 16 ] = {
	0x00000000,
	0x0FF0FF00,
	0x0FF0FF00,
	0x00000000,
	0xFFFFFF00,
	0x0FFFF000,
	0x00FF0000,
	0x00000000,

	0xFF0F0F00,
	0xF00F0F00,
	0xFF0FFF00,
	0xF0000000,
	0x000F0F00,
	0xFF0FF000,
	0xF00FF000,
	0xFF0F0F00
};

TestFramework_TestResult VDPManagerTests_verifyCtor() {
	TestFramework_TestResult testResult;
	VDPManager* vdpManager;

	vdpManager = calloc( 1, sizeof( VDPManager ) );
	VDPManager_ctor( vdpManager );

	for( int i = 0; i != 3; i++ ) {
		char expectString[ 50 ] = { 0 };

		sprintf( expectString, "palette for %d to be NULL", i );
		TestFramework_EXPECT( vdpManager->palettes[ i ].palette == NULL, expectString );

		expectString[ 0 ] = 0;

		sprintf( expectString, "tag for %d to be VDPManager_TAG_NULL", i );
		TestFramework_EXPECT( vdpManager->palettes[ i ].tag == VDPManager_TAG_NULL, expectString );
	}

	TestFramework_EXPECT( vdpManager->usedVDPSegments == NULL, "usedVDPSegments to be NULL" );
	TestFramework_EXPECT( vdpManager->usedSegmentCount == 0, "usedSegmentCount to be 0" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManagerTests_finally( vdpManager );

	return testResult;
}

TestFramework_TestResult VDPManagerTests_verifyLoadTilesFromNull() {
	TestFramework_TestResult testResult;
	VDPManager* vdpManager;

	vdpManager = calloc( 1, sizeof( VDPManager ) );
	VDPManager_ctor( vdpManager );

	// load your tiles here
	VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, TILES_BOX_DRAWING_BETA );

	TestFramework_EXPECT( vdpManager->usedSegmentCount == 1, "usedSegmentCount to be incremented by 1" );

	VDPManager_VDPRamSegment firstSegment = vdpManager->usedVDPSegments[ 0 ];
	TestFramework_EXPECT( firstSegment.tileData == BoxDrawingCharacters, "firstSegment tileData to be the box drawing characters" );
	TestFramework_EXPECT( firstSegment.index == VDPManager_TILE_USERINDEX, "firstSegment index to be VDPManager_TILE_USERINDEX" );
	TestFramework_EXPECT( firstSegment.length == 3, "firstSegment length to be 3" );
	TestFramework_EXPECT( firstSegment.tag == TILES_BOX_DRAWING_BETA, "firstSegment tag to be TILES_BOX_DRAWING_BETA" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManagerTests_finally( vdpManager );

	return testResult;
}

TestFramework_TestResult VDPManagerTests_verifyLoadSecondTile() {
	TestFramework_TestResult testResult;
	VDPManager* vdpManager;

	vdpManager = calloc( 1, sizeof( VDPManager ) );
	VDPManager_ctor( vdpManager );

	VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, TILES_BOX_DRAWING_BETA );
	VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, VDPManager_TAG_NULL );

	VDPManager_VDPRamSegment secondSegment = vdpManager->usedVDPSegments[ 1 ];

	TestFramework_EXPECT( secondSegment.tileData == BoxDrawingCharacters, "secondSegment tileData to be the box drawing characters" );
	TestFramework_EXPECT( secondSegment.index == VDPManager_TILE_USERINDEX + 3, "secondSegment index to be firstSegment.index + firstSegment.length" );
	TestFramework_EXPECT( secondSegment.length == 3, "secondSegment length to be 3" );
	TestFramework_EXPECT( secondSegment.tag == VDPManager_TAG_NULL, "secondSegment tag to be VDPManager_TAG_NULL" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManagerTests_finally( vdpManager );

	return testResult;
}

TestFramework_TestResult VDPManagerTests_verifyThirdTileNoGap() {
	TestFramework_TestResult testResult;
	VDPManager* vdpManager;

	vdpManager = calloc( 1, sizeof( VDPManager ) );
	VDPManager_ctor( vdpManager );

	VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, TILES_BOX_DRAWING_BETA );
	VDPManager_loadTiles( vdpManager, BoxDrawingCharacters, 3, VDPManager_TAG_NULL );
	VDPManager_loadTiles( vdpManager, VDPManagerTests_TestTwoTiles, 2, TILES_TWO_TILES_TEST );

	VDPManager_VDPRamSegment thirdSegment = vdpManager->usedVDPSegments[ 2 ];
	TestFramework_EXPECT( thirdSegment.tileData == VDPManagerTests_TestTwoTiles, "thirdSegment tileData to be VDPManagerTests_TestTwoTiles" );
	TestFramework_EXPECT( thirdSegment.index == VDPManager_TILE_USERINDEX + 6, "thirdSegment index to be firstSegment.index + 6" );
	TestFramework_EXPECT( thirdSegment.length == 2, "thirdSegment length to be 2" );
	TestFramework_EXPECT( thirdSegment.tag == TILES_TWO_TILES_TEST, "thirdSegment tag to be TILES_TWO_TILES_TEST" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManagerTests_finally( vdpManager );

	return testResult;
}

TestFramework_TestResult VDPManagerTests_verifyThirdTileGap() {
	TestFramework_TestResult testResult;
	VDPManager* vdpManager;

	vdpManager = calloc( 1, sizeof( VDPManager ) );
	VDPManager_ctor( vdpManager );

	// Mock VDPRamSegments
	VDPManager_pushSegment( vdpManager, BoxDrawingCharacters, VDPManager_TILE_USERINDEX, 3, 0 );
	VDPManager_pushSegment( vdpManager, BoxDrawingCharacters, VDPManager_TILE_USERINDEX + 3, 3, 0 );
	VDPManager_pushSegment( vdpManager, BoxDrawingCharacters, VDPManager_TILE_USERINDEX + 9, 3, 0 );

	// Add the two-segment indexed item which will be rearranged + flattened using qsort/qsortComparator
	VDPManager_loadTiles( vdpManager, VDPManagerTests_TestTwoTiles, 2, TILES_TWO_TILES_TEST );

	TestFramework_EXPECT( vdpManager->usedSegmentCount == 4, "four segments to have been added" );
	TestFramework_EXPECT( vdpManager->usedVDPSegments[ 0 ].index == VDPManager_TILE_USERINDEX, "first segment to be where I put it" );
	TestFramework_EXPECT( vdpManager->usedVDPSegments[ 1 ].index == VDPManager_TILE_USERINDEX + 3, "second segment to be where I put it" );
	TestFramework_EXPECT( vdpManager->usedVDPSegments[ 2 ].index == VDPManager_TILE_USERINDEX + 6, "the new tile segment to be at position userindex + 6" );
	TestFramework_EXPECT( vdpManager->usedVDPSegments[ 3 ].index == VDPManager_TILE_USERINDEX + 9, "the last segment to be where I put it" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManagerTests_finally( vdpManager );

	return testResult;
}

void VDPManagerTests_finally( VDPManager* vdpManager ) {
	VDPManager_dtor( vdpManager );
	free( vdpManager );
}

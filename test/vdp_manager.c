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
	}
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
	VDPManager_dtor( vdpManager );
	free( vdpManager );

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
	VDPManager_dtor( vdpManager );
	free( vdpManager );

	return testResult;
}

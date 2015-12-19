#include <test/vdp_manager.h>
#include <test/test_framework.h>
#include <types.h>
#include <res/globals.h>
#include <stdlib.h>
#include <stdio.h>
#include <vdpmanager.h>

const TestFramework_TestCaseDefinition VDPManagerTests[] = {
	{
		"Should properly initialize a VDPManager",
		VDPManagerTests_verifyCtor
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
	TestFramework_EXPECT( vdpManager->usedSegmentCount == 0, "usedSegmentCount to be NULL" );

	testResult = TestFramework_TestResult_TEST_PASS;

finally:
	VDPManager_dtor( vdpManager );

	return testResult;
}

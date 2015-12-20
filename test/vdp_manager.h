#ifndef VDP_MANAGER_TESTS
#define VDP_MANAGER_TESTS

#include <test/test_framework.h>
#include <vdpmanager.h>

extern const TestFramework_TestCaseDefinition VDPManagerTests[];
extern const u32 VDPManagerTests_TestTwoTiles[ 16 ];

TestFramework_TestResult VDPManagerTests_verifyCtor();
TestFramework_TestResult VDPManagerTests_verifyLoadTilesFromNull();
TestFramework_TestResult VDPManagerTests_verifyLoadSecondTile();
TestFramework_TestResult VDPManagerTests_verifyThirdTileNoGap();
TestFramework_TestResult VDPManagerTests_verifyThirdTileGap();

TestFramework_TestResult VDPManagerTests_verifyTagNullReturn();
TestFramework_TestResult VDPManagerTests_verifyAbleToReturnTag();
TestFramework_TestResult VDPManagerTests_verifyUnloadTile();

void VDPManagerTests_finally( VDPManager* vdpManager );
void VDPManagerTests_fillDemoSegments( VDPManager* vdpManager );

#endif

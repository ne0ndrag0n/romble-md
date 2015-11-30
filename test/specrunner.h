#ifndef SPECRUNNER
#define SPECRUNNER

#include <test/test_framework.h>

void SpecRunner_test();

/**
 * All execute statements go in here. Call SpecRunner_test in main.c if
 * compiled with the testing flag (makefile recipe). Instead of starting Romble,
 * the Sega Genesis will power up in testing mode and execute unit tests by
 * calling this command.
 */
void SpecRunner_caseList();

#endif

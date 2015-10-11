#ifndef BIN_SIZED_ARRAY
#define BIN_SIZED_ARRAY

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>

typedef struct BinarySizedArray {
	SizedArray super;

	/**
	 * The number of bits used from the current byte
	 */
	u8 bitsUsed;
} BinarySizedArray;

u8 BinarySizedArray_takeBits( BinarySizedArray* this, u8 bitsRequested );

#endif

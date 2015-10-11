#ifndef BIN_SIZED_ARRAY
#define BIN_SIZED_ARRAY

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>

typedef struct BinarySizedArray {
	SizedArray super;

	/**
	 * The byte currently in use (gets shifted)
	 *
	 * Always initialize to *super->items, cast to u8*
	 */
	u8 currentByte;

	/**
	 * The number of bits used from the current byte
	 *
	 * Always initialize to zero
	 */
	u8 bitsUsed;
} BinarySizedArray;

u8 BinarySizedArray_takeBits( BinarySizedArray* this, u8 bitsRequested );

#endif

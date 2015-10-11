#include <binarysizedarray.h>
#include <sizedarray.h>
#include <types.h>
#include <res/globals.h>

u8 BinarySizedArray_takeBits( BinarySizedArray* this, u8 bitsRequested ) {
	SizedArray* sizedArray = CLASS( SizedArray, this );
	u8 desired = 0;
	u8 bitsRemaining = 8 - this->bitsUsed;

	if( bitsRequested > bitsRemaining ) {
		// There is not enough bits to take from the current byte. We'll need to increment.

		// Take all remaining bits from the current byte and place them in desired.
		desired = this->currentByte & ( ( 1 << bitsRemaining ) - 1 );

		// From bitsRequested, subtract the bits we just used (which is the bits remaining in the current byte)
		bitsRequested -= bitsRemaining;

		// Increment pointer to get to next byte
		SizedArray_takeBytes( sizedArray, &( this->currentByte ), 1 );

		// Tack on the remaining bits, which come from the next byte. Shift left by the amount
		// of bits we already added, and combine by OR'ing it onto our return value.
		desired |= ( this->currentByte & ( ( 1 << bitsRequested ) - 1 ) ) << bitsRemaining;

		// The new number of bits used is "bitsRequested", which now is the number of leftover bits
		// we had to get from the next byte.
		this->bitsUsed = bitsRequested;

		// Slice off the bits we used up
		this->currentByte >>= bitsRequested;
	} else {
		// No byte increment required - we can get the required bits from the current byte.
		desired = this->currentByte & ( ( 1 << bitsRequested ) - 1 );
		// Slice off the bits we used up
		this->currentByte >>= bitsRequested;
		// Track number of bits used up in current byte
		this->bitsUsed += bitsRequested;
	}

	// desired now should be the number of bits taken from this array of bytes.
	return desired;
}

#include <sizedarray.h>
#include <types.h>
#include <stddef.h>
#include <string.h>

inline void SizedArray_takeBytes( SizedArray* this, void* destination, size_t amount ) {
	memcpy( destination, this->items, amount );

	this->items = ( ( char* ) this->items ) + amount;
	this->length =- amount;
}

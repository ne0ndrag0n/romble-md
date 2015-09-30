#ifndef SIZED_ARRAY
#define SIZED_ARRAY

#include <types.h>
#include <stddef.h>

typedef struct {
	void* items;
	u32 length;
} SizedArray;

extern void SizedArray_takeBytes( SizedArray* this, void* destination, size_t amount );

#endif

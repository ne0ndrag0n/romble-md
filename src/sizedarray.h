#ifndef SIZED_ARRAY
#define SIZED_ARRAY

#include <types.h>
#include <stddef.h>

typedef struct {
	void* items;
	u32 length;
} SizedArray;

// Take bytes from SizedArray and change the pointer
extern void SizedArray_takeBytes( SizedArray* this, void* destination, size_t amount );
// "Peek" at bytes by copying data without incrementing the pointer
extern void SizedArray_peekBytes( SizedArray* this, void* destination, size_t amount );
// "Burn" bytes by incrementing the pointer and decrementing the amount
extern void SizedArray_burnBytes( SizedArray* this, size_t amount );

#endif

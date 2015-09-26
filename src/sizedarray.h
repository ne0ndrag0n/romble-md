#ifndef SIZED_ARRAY
#define SIZED_ARRAY

#include <types.h>

typedef struct {
	void* items;
	u32 length;
} SizedArray;

#endif

#ifndef LZWUTILS
#define LZWUTILS

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>

SizedArray LZWUtils_decompress( SizedArray* lzwCode, u8 minCodeSize );
SizedArray LZWUtils_compress( SizedArray* data );

#endif

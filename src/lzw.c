#include <lzw.h>
#include <sizedarray.h>

/**
 * Looks like GIF has enough changes to the standard that this can only be
 * used for general-purpose data (unzipping ROMs.)
 *
 * For GIFs, use GifImage_decompress instead!
 */
SizedArray LZWUtils_decompress( SizedArray* lzwCode, u8 minCodeSize ) {
	SizedArray decompression = { NULL, 0 };

	return decompression;
}

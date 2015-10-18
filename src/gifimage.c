#include <gifimage.h>
#include <image.h>
#include <types.h>
#include <res/globals.h>
#include <romble.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <utility.h>
#include <stdio.h>
#include <sizedarray.h>
#include <binarysizedarray.h>
#include <vdp_pal.h>
#include <vdp.h>
#include <lang.h>
#include <kdebug.h>
#include <lzw.h>

GifImage_vtable GifImage_table = {
	GifImage_dtor,
	Image_loadData,
	GifImage_getVDPTiles
};

void GifImage_ctor( GifImage* this ) {
	Image_ctor( CLASS( Image, this ) );

	CLASS( Image, this )->functions = &GifImage_table;

	this->controlParams = ( GifImage_ControlParameters ){ 0, 0, 0 };
	this->backgroundPalIndex = 0;
	this->imageDrawn = FALSE;
};

void GifImage_dtor( GifImage* this ) {
	Image_dtor( CLASS( Image, this ) );
};

// Turn GIF image into VDP tiles
SizedArray* GifImage_getVDPTiles( GifImage* this, bool keep ) {
	SizedArray* vdpTiles = Image_getVDPTiles( CLASS( Image, this ), keep );
	SizedArray file = { NULL, 0 };

	// Clone pointer to file - This method returns a null pointer if there is no file
	if( CLASS( Image, this )->imageData != NULL ) {
		file.items = CLASS( Image, this )->imageData->items;
		file.length = CLASS( Image, this )->imageData->length;
	} else {
		return NULL;
	}

	// If this->vdpTiles is not null and keep is true, simply return what we got
	// If keep is false, clear what we got
	if( vdpTiles != NULL ) {
		if( keep == TRUE ) {
			return vdpTiles;
		} else {
			Romble_secureFree_d( ( void * ) &( vdpTiles->items ), FILE_LINE() );
			vdpTiles->length = 0;
		}
	} else {
		vdpTiles = Romble_alloc_d( sizeof( SizedArray ), TRUE, FILE_LINE() );

		if( keep == TRUE ) {
			CLASS( Image, this )->vdpTiles = vdpTiles;
		}
	}

	if ( GifImage_isGifImage( &file ) == TRUE ) {
		SizedArray_takeBytes( &file, &( CLASS( Image, this )->width ), 2 );
		CLASS( Image, this )->width = Utility_swapu16( CLASS( Image, this )->width );

		SizedArray_takeBytes( &file, &( CLASS( Image, this )->height ), 2 );
		CLASS( Image, this )->height = Utility_swapu16( CLASS( Image, this )->height );

		u8 packedField = 0;
		SizedArray_takeBytes( &file, &packedField, 1 );

		SizedArray_takeBytes( &file, &( this->backgroundPalIndex ), 1 );

		// Burn the aspect ratio as it's irrelevant
		SizedArray_burnBytes( &file, 1 );

		// Handle the palette (if any)
		GifImage_buildPalette( this, &file, packedField );

		// Handle looping chunks depending on type
		u8 chunkType = 0;
		u8 extensionType = 0;
		while( chunkType != GifImage_TRAILER ) {
			// Take block header
			SizedArray_takeBytes( &file, &chunkType, 1 );
			switch( chunkType ) {
				case GifImage_EXTENSION:
					SizedArray_takeBytes( &file, &extensionType, 1 );
					switch( extensionType ) {
						case GifImage_EXTENSION_CONTROL:
							// Burn the block length (always 04)
							SizedArray_burnBytes( &file, 1 );
							GifImage_loadControlParameters( this, &file );
							break;
					}
					break;
				case GifImage_IMAGE_SEPARATOR:
					GifImage_processImage( this, &file );
					chunkType = GifImage_TRAILER;
					break;
				default:
					// Prevent an infinite loop - if you don't recognize the byte, GET OUT!
					chunkType = GifImage_TRAILER;
					break;
			}
		}

	}

	// If the status is not an error status, apply "OK" at this point
	if( CLASS( Image, this )->imageStatus == Image_Status_UNBUILT ||
		CLASS( Image, this )->imageStatus == Image_Status_OK ) {
		CLASS( Image, this )->imageStatus = Image_Status_OK;
	}

	return vdpTiles;
}

bool GifImage_isGifImage( SizedArray* file ) {
	char header[7] = { 0 };
	SizedArray_takeBytes( file, header, 6 );

	return ( strcmp( header, GifImage_GIF87a ) == 0 || strcmp( header, GifImage_GIF89a ) == 0 );
}

void GifImage_buildPalette( GifImage* this, SizedArray* file, u8 packedField ) {
	bool palettePresent = ( packedField & GifImage_PALETTE_PRESENT_MASK ) >> 7;
	u8 paletteSize = ( packedField & GifImage_PALETTE_SIZE_MASK );
	u16 numPaletteEntries; // Needs to fit the number 256

	if( palettePresent == TRUE ) {
		// Conducts the operation 2^n+1
		numPaletteEntries =  1 << ( paletteSize + 1 );

		// Store the native palette as a u8 SizedArray - first clear out existing SizedArray
		if( CLASS( Image, this )->nativePalette != NULL ) {
			Romble_secureFree_d( ( void* ) &( CLASS( Image, this )->nativePalette->items ), FILE_LINE() );
			Romble_secureFree_d( ( void* ) &( CLASS( Image, this )->nativePalette ), FILE_LINE() );
		}

		// Allocate room for SizedArray, memcpy bytes into the SizedArray
		CLASS( Image, this )->nativePalette = Romble_alloc_d( sizeof( SizedArray ), TRUE, FILE_LINE() );
		// Each palette entry for GIF is three bytes
		CLASS( Image, this )->nativePalette->length = numPaletteEntries * 3;
		CLASS( Image, this )->nativePalette->items = Romble_alloc_d( CLASS( Image, this )->nativePalette->length * sizeof( u8 ), TRUE, FILE_LINE() );
		Romble_assert( CLASS( Image, this )->nativePalette->items != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
		SizedArray_takeBytes( file, CLASS( Image, this )->nativePalette->items, CLASS( Image, this )->nativePalette->length );

		// Determine what palette to generate based on this Image class's PaletteMode.
		// Because an PaletteMode may change, set the image's PaletteMode after each call.
		switch( CLASS( Image, this )->paletteMode ) {
			//case Image_PaletteMode_OCTREE:
				// Convert a non-indexed image or image > 16 entries.
				// Drop down to NATIVE_IMAGE if the image already has less than or equal to 16 indexed colours
				//CLASS( Image, this )->paletteMode = Image_PaletteMode_OCTREE;
				//break;
			default:
			case Image_PaletteMode_NATIVE_IMAGE:
				// Simply convert this image's native palette to Sega format if there are 16 or fewer palette entries.
				// Drop down to NEAREST_DEFAULT if the image is non-indexed or has > 16 palette entries.
				Romble_secureFree_d( ( void* ) &( CLASS( Image, this )->palette ), FILE_LINE() );
				CLASS( Image, this )->palette = Image_RGBtoSega( CLASS( Image, this )->nativePalette );
				CLASS( Image, this )->paletteMode = Image_PaletteMode_NATIVE_IMAGE;
				// just a test
				//VDP_setPalette( PAL1, CLASS( Image, this )->palette );
				break;
			//case Image_PaletteMode_NEAREST_DEFAULT:
				// Translate each tile to match Romble's default palette, PAL0.
				// This does nothing in this function; buildTiles will take this up.
				//CLASS( Image, this )->paletteMode = Image_PaletteMode_NEAREST_DEFAULT;
				//break;
		}
	}
}

void GifImage_loadControlParameters( GifImage* this, SizedArray* file ) {
	SizedArray_takeBytes( file, &( this->controlParams  ), 4 );

	// Correct endianness of delayTime
	this->controlParams.delayTime =  Utility_swapu16( this->controlParams.delayTime );

	// Burn the terminator
	SizedArray_burnBytes( file, 1 );

}

/**
 * Process a GIF subimage after the image descriptor. Pass off control to decompress using
 * GifImage_decompress, and then depending on this->paletteMode,  recompile the image to
 * a format suitable for the Sega VDP.
 *
 * @param		{GifImage*}		this
 * @param		{SizedArray*}	file		The "file" of the GIF image
 */
void GifImage_processImage( GifImage* this, SizedArray* file ) {
	GifImage_ImageDescriptor imageDescriptor;
	u16 segmentWidth = CLASS( Image, this )->width;
	u16 segmentHeight = CLASS( Image, this )->height;

	// Fill struct with the rest of the image descriptor
	SizedArray_takeBytes( file, &imageDescriptor, 9 );

	// Fix endianness
	imageDescriptor.imageLeft = Utility_swapu16( imageDescriptor.imageLeft );
	imageDescriptor.imageTop = Utility_swapu16( imageDescriptor.imageTop );
	imageDescriptor.imageWidth = Utility_swapu16( imageDescriptor.imageWidth );
	imageDescriptor.imageHeight = Utility_swapu16( imageDescriptor.imageHeight );

	if( ( ( imageDescriptor.options & GifImage_INTERLACE_MASK ) >> 6 ) == TRUE ) {
		// FIXME: Version 0.0.3, we do not currently support an interlaced GIF.
		Debug_print( "Interlaced images are not yet implemented!" );
		CLASS( Image, this )->imageStatus = Image_Status_FEATURE_UNSUPPORTED;
		return;
	}

	// If the first image has a local color table, rebuild + reset the palette and mode
	// This will be ignored if there's not another color table
	GifImage_buildPalette( this, file, imageDescriptor.options );

	// If for some reason the image is offset by an amount, subtract the width and height by the difference
	u8 segmentPadding = 0;
	segmentWidth -= imageDescriptor.imageLeft;
	segmentPadding = segmentWidth % 8;
	if( segmentPadding != 0 ) {
		segmentWidth += 8 - segmentPadding;
	}
	segmentHeight -= imageDescriptor.imageTop;
	segmentPadding = segmentHeight % 8;
	if( segmentPadding != 0 ) {
		segmentHeight += 8 - segmentPadding;
	}
	CLASS( Image, this )->paddedWidth = segmentWidth;
	CLASS( Image, this )->paddedHeight = segmentHeight;

	// Create the u32s for vdpTiles
	CLASS( Image, this )->vdpTiles->length = ( segmentWidth * segmentHeight ) / 8;
	CLASS( Image, this )->vdpTiles->items = Romble_alloc_d( sizeof( u32 ) * CLASS( Image, this )->vdpTiles->length, TRUE, FILE_LINE() );

	// Time for the beef: the LZW compression!
	u8 minCodeSize, sequenceLength;

	// Read minimum code size
	SizedArray_takeBytes( file, &minCodeSize, 1 );

	// This is where we left off
	u32 currentPixel = 0;

	do {
		// Read in the sequence length
		SizedArray_takeBytes( file, &sequenceLength, 1 );

		// If the sequence length is greater than zero,
		// construct an uncompressed block from the
		// byte sequence, and then concatenate this
		// onto decompressedSequence. decompressedSequence
		// will be transformed depending on image mode,
		// then copied to this->vdpTiles and deallocated.
		if( sequenceLength > 0 ) {
			// Properly set up a BinarySizedArray
			BinarySizedArray compressedBlock;
			compressedBlock.super.items = file->items;
			compressedBlock.super.length = sequenceLength;
			SizedArray_takeBytes( &( compressedBlock.super ), &( compressedBlock.currentByte ), 1 );
			compressedBlock.bitsUsed = 0;

			// Burn the length of the coded sequence
			SizedArray_burnBytes( file, sequenceLength );

			// Beef of the work is done in this function
			// Going back on the plan to decompress and return SizedArray - instead,
			// dictionary is only kept memory-resident, and the image will be built
			// on-the-fly.
			GifImage_decompress( this, &compressedBlock, minCodeSize, &currentPixel );
		}

	} while( sequenceLength > 0 );
}

/**
 * Decompresss the code sequence in a GIF image. This function should likewise
 * build the VDP tiles on-the-fly.
 *
 * @param	{GifImage*}			this
 * @param	{BinarySizedArray*}	compressedBlock		A compressed image block, to be un-LZW'd
 * @param	{u8}				minCodeSize			The minimum code size. 2**minCodeSize
 * 													determines the position of the clear code
 * 													and the EOF code. This number is always
 * 													the bits per pixel of the image.
 * @param	{u32*)				lastPixelDecoded	Byref the last pixel we operated on in the stream
 * 													of VDP u32s.
 */
void GifImage_decompress( GifImage* this, BinarySizedArray* compressedBlock, u8 minCodeSize, u32* lastPixelDecoded ) {
	SizedArray image = { NULL, 0 };
	// Dictionary is a SizedArray of SizedArrays
	SizedArray dictionary = { NULL, 0 };
	u16 currentCodeSize = minCodeSize + 1;

	// Build dictionary as we go, examine one code at a time
	u16 currentCode = 0;

	// Get tile numbers in both directions
	u16 tilesX = CLASS( Image, this )->paddedWidth / 8;
	u16 tilesY = CLASS( Image, this )->paddedHeight / 8;

	// Now initialize the code table (do this every time we encounter the clear code)
	GifImage_buildCodeTable( &dictionary, minCodeSize );

	//( ( u16* )( ( ( SizedArray* )( dictionary.items ) )[ 5 ].items ) )[ 0 ];

	// BinarySizedArray compressedBlock should now be ready for the bit-taking!

	// Current iteration pattern for a 2x2 image:
	// 0, 8, 1, 9, ... , 7, 15
	// next row:
	// 16, 24, 17, 25, ... , 23, 31
	// j * 8 + i, where i iterates from 0 to 7 and j iterates from 0 to tiles_x
	// outer loop can compute a difference required in the form of ( 8 * tiles_x * h )

	for( u16 row = 0; row < tilesY; row++ ) {
		for( u8 subrow = 0; subrow != 8; subrow++ ) {
			u16 startPoint = 8 * tilesX * row;

			for( u16 tile = 0; tile < tilesX; tile++ ) {
				u32 selected = ( ( u32* )CLASS( Image, this )->vdpTiles->items )[ ( ( tile * 8 ) + subrow ) + startPoint ];

				// Selected should give us the correct number from the array of u32's
				// Now, output nibbles from the LZW code stream

			}
		}
	}

	// Deallocate dictionary items AND the items within the dictionary
}

/**
 * Rebuild the code table to its default position.
 */
void GifImage_buildCodeTable( SizedArray* dictionary, u8 minCodeSize ) {

	// maxInitialized - 2^minCodeSize + 2, the extra two are the clear code and end of sequence code
	u16 maxInitialized = ( 1 << minCodeSize ) + 2;

	for( u16 i = 0; i != maxInitialized; i++ ) {
		// Allocate the amount of new SizedArrays; this will create one more SizedArray than currently exists
		SizedArray* items = Romble_realloc_d( dictionary->items, sizeof( SizedArray ) * ( i + 1 ), FILE_LINE() );
		Romble_assert( items != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
		dictionary->items = items;
		dictionary->length++;

		SizedArray* next = items + i;
		next->items = Romble_alloc( sizeof( u16 ), TRUE );
		Romble_assert( next->items != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
		( ( u16* )( next->items ) )[ 0 ] = i;
		next->length = 1;
	}
}

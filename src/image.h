#ifndef IMAGE
#define IMAGE

#include <res/globals.h>
#include <types.h>
#include <sizedarray.h>

#define		Image_U8_UPPER_NIBBLE		0xF0
#define		Image_U8_LOWER_NIBBLE		0x0F
#define		Image_MAXIMUM_ROUNDABLE		0x0E
#define		Image_MAXIMUM_RGB_NIBBLE	0x0F
#define		Image_SEGA_PAL_SIZE			16

struct Image;

typedef void* Image_nativePalette;

typedef struct {
	void ( *destroy )( struct Image* );
	void ( *loadData )( struct Image*, SizedArray* items );
	SizedArray* ( *getVDPTiles )( struct Image*, bool );
} Image_vtable;

typedef enum {
	/**
	 * @notImplemented
	 *
	 * NEAREST_DEFAULT returns a modified set of tiles. Each pixel's colours are selected
	 * based on closeness to Romble's reserved PAL0, which contains the standard, DOS-era
	 * EGA palette. If the image is indexed, dereference the palette entry and select
	 * what is closest to that. If the image is non-indexed, take the RGB from the pixel
	 * and select the PAL0 entry closest to that. The formula used is based on HSV.
	 *
	 * DOES return a modified set of tiles. Each pixel points to the closest PAL0 col for it.
	 * DOES NOT create a this->palette.
	 */
	Image_PaletteMode_NEAREST_DEFAULT,

	/**
	 * NATIVE_IMAGE simply uses the tiles and image palette as-is. For this option to take
	 * effect, the image must be an indexed image containing 16 or fewer palette entries.
	 * If the image does not meet these conditions, this option falls back to
	 * NEAREST_DEFAULT.
	 *
	 * NATIVE_IMAGE is the default palette mode. Unless specified otherwise, all images
	 * will attempt to use a native 16-or-fewer palette. They fall back to finding the
	 * nearest PAL0 entry if we are unable to use the native palette. If a more graceful
	 * degredation is required, a developer may use OCTREE as an alternative, but at
	 * higher computational cost (see below).
	 *
	 * DOES NOT return a modified set of tiles.
	 * DOES create a this->palette. Sega-compatible entries based on the native colours.
	 * FALLS BACK to NEAREST_DEFAULT if image is non-indexed, or contains > 16 pal entries.
	 */
	Image_PaletteMode_NATIVE_IMAGE,

	/**
	 * @notImplemented
	 *
	 * OCTREE is a variation of NATIVE_IMAGE that is able to reduce higher-colour images
	 * down to a palette of 16 using octrees. Rather than falling back to NEAREST_DEFAULT
	 * if the image contains greater than 16 palette entries, it generates a new palette
	 * consisting of entries needed to best depict the Image in 16-colour. If the image
	 * does not require any palette reduction, OCTREE falls back to NATIVE_IMAGE.
	 *
	 * Use this entry with care, as it is very memory and CPU-intensive. This mode is
	 * intended for unconverted images used directly from an outside source that Romble
	 * would use in theory. It is recommended, for maximum performance, that a Romble
	 * repository provides an image directly usable in NATIVE_IMAGE mode. For performance
	 * reasons, NATIVE_IMAGE falls back on NEAREST_DEFAULT rather than OCTREE for
	 * incompatible images.
	 *
	 * DOES return a modified set of tiles.
	 * DOES create a this->palette.
	 * FALLS BACK to NATIVE_IMAGE if the image does not require reduction.
	 */
	Image_PaletteMode_OCTREE
} Image_PaletteMode;

typedef enum {
	/**
	 * The image is OK to be used and this->vdpTiles are ready to be inserted into the Genesis VDP.
	 */
	Image_Status_OK,

	/**
	 * The image's VDP tiles have not yet been built.
	 */
	Image_Status_UNBUILT,

	/**
	 * An unexpected end of file was encountered while reading the Image file.
	 */
	Image_Status_UNEXPECTED_EOF,

	/**
	 * An essential feature of this image format is currently unsupported.
	 */
	Image_Status_FEATURE_UNSUPPORTED,

	/**
	 * The file format was checked and found invalid.
	 */
	Image_Status_INVALID_FILE
} Image_Status;

typedef struct Image {
	SizedArray* imageData;
	SizedArray* vdpTiles;
	SizedArray* nativePalette;

	Image_Status imageStatus;

	u16* palette;
	Image_PaletteMode paletteMode;

	u16 width;
	u16 height;

	vtable functions;
} Image;

void Image_ctor( Image* this );
void Image_dtor( Image* this );
void Image_loadData( Image* this, SizedArray* file );
SizedArray* Image_getVDPTiles( Image* this, bool keep );

u16* Image_RGBtoSega( SizedArray* rgbTriplets );

#endif

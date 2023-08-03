#pragma once

#include <ldv/color.h>

namespace d2d { namespace world {

using color=ldv::rgba_color;

struct collision_tile;
/**
 * a little interface to tell our tiles how to behave
 */
struct collision_tile_implementation {
	public:

/**
 * given a tile type as defined by the application, what shape of the 
 * application shaper corresponds to it? Both parts communicate through the
 * integer primitive.
 */
	virtual int             to_shape(const collision_tile&) const=0;

/**
*what would the colors be for this tile? The tile itself is passed along so
*we can use its properties.
*/
	virtual color           get_outline_color(const collision_tile&) const=0;
	virtual color           get_fill_color(const collision_tile&) const=0;
};

}}

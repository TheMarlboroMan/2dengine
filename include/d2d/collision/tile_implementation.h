#pragma once

#include <ldv/color.h>

namespace d2d { namespace collision {

using color=ldv::rgba_color;

struct tile;
/**
 * a little interface to tell our tiles how to behave
 */
struct tile_implementation {
	public:

/**
 * given a tile type as defined by the application, what shape of the 
 * application shaper corresponds to it? Both parts communicate through the
 * integer primitive.
 */
	virtual int             to_shape(const tile&) const=0;

/**
 * given a tile, is it passable from the given edge?
 */
	virtual bool            is_passable_edge(const tile&, box_edge) const=0;

/**
*what would the colors be for this tile? The tile itself is passed along so
*we can use its properties. These colors would be only used for the debug
*display.
*/
	virtual color           get_outline_color(const tile&) const=0;
	virtual color           get_fill_color(const tile&) const=0;
};

}}

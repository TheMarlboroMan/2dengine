#pragma once

#include "tile.h"

namespace d2d { namespace collision {

/**
 * the interface for something that can find tiles given coordinates. Usually
 * the map loader would load tiles as a vector of them. Implementing this
 * interface will allow for a different way to access them.
 */
class tile_finder {

	public:

/**
 * must return the tile at x,y, null if no tile is in that position
 */
	virtual d2d::collision::tile*       get(int, int) const=0;
/**
 * must return true if there is a tile at x,y
 */
	virtual bool                        has(int, int) const=0;
};
}}

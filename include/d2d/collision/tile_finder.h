#pragma once

#include "tile.h"

#include <ostream>

namespace d2d { namespace collision {

/**
 * the interface for something that can find tiles given coordinates. Usually
 * the map loader would load tiles as a vector of them. Implementing this
 * interface will allow for a different way to access them.
 */
class tile_finder {

	public:

/**
 * must return the tile at x,y, null if no tile is in that position. X
 * and Y are in tile units and not in world space units.
 */
	virtual const d2d::collision::tile* get(int, int)const=0;
	virtual d2d::collision::tile*       get(int, int)=0;
/**
 * must return true if there is a tile at x,y. X and Y are in tile units
 * and not in world space units.
 */
	virtual bool                        has(int, int)const=0;
};

}}

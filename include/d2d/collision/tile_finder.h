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

	struct limits{
		int left, right, //leftmost and rightmost x indexes, inclusive.
	        bottom, top, //bottommost and topmost y indexes, inclusive.
	        w, h;
	};

/**
 * must return the tile at x,y, null if no tile is in that position
 */
	virtual const d2d::collision::tile* get(int, int)const=0;
	virtual d2d::collision::tile*       get(int, int)=0;
/**
 * must return true if there is a tile at x,y
 */
	virtual bool                        has(int, int)const=0;
/**
 * must return a limits struct.
 */
	virtual limits                      get_limits() const=0;
};

std::ostream& operator<<(std::ostream&, const tile_finder::limits&);
}}

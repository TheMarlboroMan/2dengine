#pragma once

#include "tile.h"

namespace d2d { namespace collision {


struct tile_limits{
	int left, right, //leftmost and rightmost x indexes, inclusive, in tile units.
		bottom, top; //bottommost and topmost y indexes, inclusive, in tile units.
	unsigned w, h; //width and height, in tile units.

	operator bool() const;
};

std::ostream& operator<<(std::ostream&, const tile_limits&);
/**
 * given an array of tiles, this class calculates the lower and upper bounds
 * of their coordinates in a cartesian space.
 */
class tile_limits_finder {

	public:

/**
 * calculates the upper and lower bounds on the tiles in a result object.
 * A result with all zero values will be returned if there are no tiles.
 */
	tile_limits find_limits(const std::vector<d2d::collision::tile>&) const;
};

}}

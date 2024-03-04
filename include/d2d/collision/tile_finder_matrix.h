#pragma once

#include "tile_limits.h"
#include "tile_finder.h"

namespace d2d { namespace collision {

/***
 * implementation of tile finder that stores all its data in a vector, easing
 * lookup
 */
class tile_finder_matrix:
	public tile_finder {

	public:

	                                    tile_finder_matrix(int, int);

/**
 *loads tiles into the lookup data structure. Needs the width and height of
*a full tile to be able to calculate positions.
 */
	void                                load(const std::vector<tile>&);

/**
 *empties the lookup data structure.
 */
	void                                clear();

//begin implementation of tile_finder.

	virtual tile_coords                 to_tile_coords(d2d::collision::point)const;
	virtual d2d::collision::point       to_point(tile_coords)const;
	virtual const d2d::collision::tile* get(tile_coords) const;
	virtual d2d::collision::tile*       get(tile_coords);
	virtual bool                        has(tile_coords) const;

//end implementation of tile finder.

	private:

	std::vector<d2d::collision::tile *> data;
	int                                 offset_x{0},
	                                    offset_y{0},
	                                    tile_w{0},
	                                    tile_h{0};
	tile_limits                         limit{0,0,0,0,0,0};
};
}}

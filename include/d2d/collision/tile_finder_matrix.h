#pragma once

#include "tile_finder.h"

namespace d2d { namespace collision {

/***
 * implementation of tile finder that stores all its data in a vector, easing
 * lookup
 */
class tile_finder_matrix:
	public tile_finder {

	public:

/**
 *loads tiles into the lookup data structure.
 */
	void                                load(const std::vector<tile>&);

/**
 *empties the lookup data structure.
 */
	void                                clear();

//begin implementation of tile_finder.

	virtual d2d::collision::tile*       get(int, int) const;

	virtual bool                        has(int, int) const;

//end implementation of tile finder.

	private:

	std::vector<d2d::collision::spatiable *> data;
	int                                 offset_x{0},
										offset_y{0};
};
}}

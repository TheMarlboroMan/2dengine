#pragma once

#include "definitions.h"

namespace d2d { namespace collision {

/**
 * interface for a class that holds and can return the boxes of different
 * tile types.
 */
class shaper {

	public:

	enum tiles {
		tile_full=1,
		tile_half_bottom,
		tile_half_top,
		tile_half_left,
		tile_half_right,
		tile_quarter_bottom_left,
		tile_quarter_bottom_right,
		tile_quarter_top_left,
		tile_quarter_top_right
	};

	virtual                         ~shaper() {}

	/**
	* Retrieves and repositions a prototype box according to type, x and y (in
	* tile coordinates, not world coordinates) plus a variant.
	*/
	virtual d2d::collision::box&    box(int, int, int)=0;

/**
 * width of the tiles
 */
	virtual int                     get_tile_w() const=0;
/**
 * height of the tiles
 */
	virtual int                     get_tile_h() const=0;
};

}}

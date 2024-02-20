#pragma once

#include "definitions.h"

namespace d2d { namespace collision {

/**
 * interface for a class that holds and can return the boxes of different
 * tile types.
 */
class shaper {

	public:

	virtual                         ~shaper() {}

	/**
	* Retrieves and repositions a prototype box according to type, x and y (in
	* tile coordinates, not world coordinates).
	*/
	virtual d2d::collision::box&    get_box(int, int, int)=0;

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

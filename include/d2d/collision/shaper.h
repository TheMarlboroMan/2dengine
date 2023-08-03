#pragma once

#include "definitions.h"

namespace d2d { namespace collision {

/**
* Tile shapes are repeated over and over. This class holds a collection of 
* common shapes that tiles can retrieve so we don't have to deal with the memory
* of hundreds of boxes, just with their common shapes.
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

/**
*shaper constructor, receives width and height of the basic tile.
*/
	                        shaper(int, int);

	/**
	* Retrieves and repositions a prototype box according to type, x and y (in
	* tile coordinates, not world coordinates) plus a variant.
	*/
	d2d::collision::box&         get_box(int, int, int);

/**
 * width of the tiles
 */
	const int                    tile_w;
/**
 * height of the tiles
 */
	const int                    tile_h;

	private:

	d2d::collision::box          full_box,
	                        half_box_wide,
	                        half_box_tall,
	                        quarter_box;
};

}}

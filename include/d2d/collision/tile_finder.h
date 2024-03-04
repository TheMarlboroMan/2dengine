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
 * must convert between tile and world coordinates. To tile_coord must return
 * the tile a point is in. In a tile space 16x16 units per tile the points 0,0 
 * and 15,15 lay in the tile 0,0 but 16,16 or 18,18 slay on 1,1. To point will return 
 * the lowest point (1,1 will return 16,16).
 */
	virtual tile_coords                 to_tile_coords(d2d::collision::point)const=0;
	virtual d2d::collision::point       to_point(tile_coords)const=0;
	

/**
 * must return the tile at x,y, null if no tile is in that position. X
 * and Y are in tile units and not in world space units.
 */
	virtual const d2d::collision::tile* get(tile_coords)const=0;
	virtual d2d::collision::tile*       get(tile_coords)=0;
/**
 * must return true if there is a tile at x,y. X and Y are in tile units
 * and not in world space units.
 */
	virtual bool                        has(tile_coords)const=0;
};

}}

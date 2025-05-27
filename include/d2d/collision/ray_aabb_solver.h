#pragma once

#include "ray_aabb_finder.h"

namespace d2d { namespace collision {

class ray_aabb_solver {

	public:

/**
 * TODO: Document this...
 * TODO: WILL CHANGE THE VECTOR
 * TODO: This return type is temporary...
 * Returns edges as spatiable collided with N edge of something.
 */
	int        solve(spatiable&, std::vector<ray_aabb_info>&) const;

};
}}

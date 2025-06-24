#pragma once

#include "ray_aabb_finder.h"

namespace d2d { namespace collision {

class ray_aabb_solver {

	public:

	struct result{
		//aabb_edges as defined in this library, meaning the spatiable collided
		//with the edge X of something.
		int edges,
		//count of obstacles we really collided with.
		count; 
	};

	enum flags {

	};

/**
 * Returns edges as spatiable collided with N edge of something. The vector
 * WILL BE CHANGED (as in sorted) using the default sort by intersection time.
 * This may not be enough in perfect corner collisions!
 */
	result  sort_and_solve(spatiable&, std::vector<ray_aabb_info>&) const;

/**
 * Returns edges as spatiable collided with N edge of something. The vector
 * will not be changed and is assumed to be sorted as the application needs.
 */
	result  solve(spatiable&, const std::vector<ray_aabb_info>&) const;

};
}}

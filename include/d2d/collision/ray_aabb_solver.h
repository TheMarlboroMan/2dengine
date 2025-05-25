#pragma once

#include "ray_aabb_finder.h"

namespace d2d { namespace collision {

class ray_aabb_solver {

	public:

	enum tedges             {top=1, bottom=2, right=4, left=8};

/**
 * TODO: Document this...
 * TODO: WILL CHANGE THE VECTOR
 * TODO: This return type is temporary...
 */
	int        solve(spatiable&, std::vector<ray_aabb_info>&) const;

};
}}

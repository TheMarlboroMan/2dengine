#pragma once

#include "spatiable.h"
#include "definitions.h"
#include "../motion/definitions.h"

namespace d2d { namespace collision {

class ray_builder {

	public:

/**
 * builds the ray for the spatiable and vector. This gives the full ray, not 
 * taking into account any delta time (multiply by delta to know the ray for
 * the current tic of a spatiable!).
 */
	ray     get(const spatiable&, const d2d::motion::motion_vector&) const;
	ray     get(const spatiable&) const;
/**
 * builds the ray for the previous position of the spatiable. Same delta time
 * considerations as above apply.
 */
	ray     get_previous(const spatiable&, const d2d::motion::motion_vector&) const;
	ray     get_previous(const spatiable&) const;
};

}}

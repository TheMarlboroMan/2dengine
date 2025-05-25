#pragma once

#include "spatiable.h"
#include "definitions.h"
#include "../motion/definitions.h"

namespace d2d { namespace collision {

class ray_builder {

	public:

/**
 * builds the ray for the spatiable and vector.
 */
	ray     get(const spatiable&, const d2d::motion::motion_vector&) const;
/**
 * builds the ray for the previous position of the spatiable.
 */
	ray     get_previous(const spatiable&, const d2d::motion::motion_vector&) const;
};

}}

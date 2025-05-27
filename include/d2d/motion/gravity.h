#pragma once

#include "definitions.h"
#include <ldtools/time_definitions.h>

namespace d2d { namespace collision {

class spatiable;
}}

namespace d2d { namespace motion {

struct gravity {

	                            gravity();
	                            gravity(const d2d::motion::motion_vector&, double);
	                            void apply_to(d2d::motion::motion_vector&, ldtools::tdelta);
	                            void apply_to(d2d::collision::spatiable&, ldtools::tdelta);

	d2d::motion::motion_vector  force;
	double                      max_velocity;
};
}}

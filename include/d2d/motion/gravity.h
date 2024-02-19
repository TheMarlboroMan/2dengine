#pragma once

#include "definitions.h"

namespace d2d { namespace motion {

struct gravity {

	                            gravity();
	                            gravity(const d2d::motion::motion_vector&, double);
	                            void apply_to(d2d::motion::motion_vector&, float);

	d2d::motion::motion_vector  force;
	double                      max_velocity;
};
}}

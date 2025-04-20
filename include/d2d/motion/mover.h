#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/spatiable.h>
#include <ldtools/time_definitions.h>

namespace d2d { namespace motion {

class mover {

	public:

	void        apply(d2d::collision::spatiable&, const d2d::motion::motion_vector&, ldtools::tdelta);
	void        apply_x(d2d::collision::spatiable&, double, ldtools::tdelta);
	void        apply_y(d2d::collision::spatiable&, double, ldtools::tdelta);
};
}}

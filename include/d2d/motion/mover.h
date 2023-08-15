#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/spatiable.h>

namespace d2d { namespace motion {

class mover {

	public:

	void        apply(d2d::collision::spatiable&, const d2d::motion::motion_vector&, float);
	void        apply_x(d2d::collision::spatiable&, double, float);
	void        apply_y(d2d::collision::spatiable&, double, float);
};
}}

#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/spatiable.h>
#include <ldtools/time_definitions.h>

namespace d2d { namespace motion {

class mover {

	public:

	/**
	* Applies movement vector, does not confirm previous box
	*/
	void        apply(d2d::collision::spatiable&, const d2d::motion::motion_vector&, ldtools::tdelta);
	void        apply_x(d2d::collision::spatiable&, double, ldtools::tdelta);
	void        apply_y(d2d::collision::spatiable&, double, ldtools::tdelta);
	/**
    *Applies movement and confirms box. Beware, since collision solving routines
	*may depend on the previous box to yield reasonable results.
    */
	void        apply_and_commit(d2d::collision::spatiable&, const d2d::motion::motion_vector&, ldtools::tdelta);
};
}}

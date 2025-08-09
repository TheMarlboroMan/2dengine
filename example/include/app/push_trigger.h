#pragma once

#include "entity.h"
#include <d2d/components/timeout.h>
#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <ldtools/time_definitions.h>
#include <ostream>

namespace app {

class push_trigger {

	public:

	                                push_trigger(const d2d::collision::box&, d2d::motion::motion_vector);

	bool                            is_active() const;
	void                            tic(ldtools::tdelta);
	void                            activate();

	entity                          ent;
	d2d::motion::motion_vector      velocity; //velocity it will imprint.

	private:

	d2d::components::timeout        timeout;

};

std::ostream& operator<<(std::ostream&, const push_trigger&);
}

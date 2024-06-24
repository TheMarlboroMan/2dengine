#pragma once

#include "entity.h"
#include <iostream>

namespace app {

class touch_trigger {

	public:

	                    touch_trigger(const d2d::collision::box&, int, int, bool);

	entity                          ent;
	int                             id;
	int                             tag;
	bool                            used;
};

std::ostream& operator<<(std::ostream&, const touch_trigger&);
}

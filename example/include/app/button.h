#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * cannot be named "switch".
 */
class button {

	public:

	enum types {
		regular,
		yellow_keyhole,
		blue_keyhole,
		red_keyhole
	};

	                                button(d2d::collision::point, int, int);

	entity                          ent;
	int                             type;
	int                             tag;
};

std::ostream& operator<<(std::ostream&, const button&);
}

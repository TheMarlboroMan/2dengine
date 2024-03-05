#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * cannot be named "switch".
 */
class button {

	public:

	enum class types {
		regular,
		yellow_keyhole,
		blue_keyhole,
		red_keyhole
	};

	                                button(d2d::collision::point, types, int, int, bool);

	entity                          ent;
	types                           type;
	int                             id;
	int                             tag;
	bool                            used;
};

std::ostream& operator<<(std::ostream&, const button&);
}

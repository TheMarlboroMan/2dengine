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
		red_keyhole,
		green_keyhole
	};

	                                button(d2d::collision::point, types, int, int, bool, bool);
	void                            reset();

	entity                          ent;
	types                           type;
	int                             id;
	int                             tag;
	bool                            used,
	//Stay "used" when the level resets or is re-entered. For persistent buttons.
	                                keep_used_when_reset;
};

std::ostream& operator<<(std::ostream&, const button&);
}

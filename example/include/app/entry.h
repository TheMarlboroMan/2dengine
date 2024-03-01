#pragma once

#include "entity.h"
#include <d2d/collision/definitions.h>

namespace app {

class entry {

	public:

	enum position {
		center_bottom=0,
		inner_top_edge=1,
		inner_right_edge=2,
		inner_bottom_edge=3,
		inner_left_edge=4
	};

/**
 * parameter order is box, id and position.
 */
	                        entry(d2d::collision::box, int, int);

	app::entity             ent;
	int                     id,
		                    position;
};

std::ostream& operator<<(std::ostream&, const entry&);
}

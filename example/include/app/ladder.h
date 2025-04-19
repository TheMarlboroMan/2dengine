#pragma once

#include <d2d/collision/motion_axis_limiter.h>
#include <iostream>

namespace app {

/**
 * behaves like a ladder, can look like a ladder, chain or vine.
 */
class ladder:
	public d2d::collision::motion_axis_limiter {

	public:

	enum types {

		t_ladder=0,
		t_chain=1,
		t_vine=2,
		t_chain_secret=3
	};

/**
 * arguments are height in world units and type
 */
	                                        ladder(d2d::collision::point, int, int);

	int                                     type;
};

std::ostream& operator<<(std::ostream&, const ladder&);
}

#pragma once

#include <d2d/collision/definitions.h>
#include <ostream>

namespace app {

/**
 * Boss skulls will appear at the points defined by this entity.
 */
class boss_skull_spawn {

	public:

	                        boss_skull_spawn(d2d::collision::point, int);

	//I'll just take the easy way and make these public.
	d2d::collision::point   point;
	int                     id;
};

std::ostream& operator<<(std::ostream&, const boss_skull_spawn&);
}

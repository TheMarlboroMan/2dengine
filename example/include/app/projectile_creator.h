#pragma once

#include "projectile.h"
#include <ldtools/time_definitions.h>
#include <d2d/collision/definitions.h>

namespace app {

/**
 * Creates projectiles and places them in a projectile vector.
 */
class projectile_creator {

	public:

	using point=d2d::collision::point;

	projectile  create_horizontal(point, double) const;
	projectile  create_vertical(point, double) const;
	projectile  create_falling(point, double) const;
	projectile  create_directed(point, double, point, int=0) const;

	//TODO; Why should these be in the executable when we can have a table
	//for these kind of things? They would not be const, I guess...
	static const int        projectile_horizontal_w{7};
	static const int        projectile_horizontal_h{7};
	static const int        projectile_horizontal_desintegration_ms{300};

	static const int        projectile_vertical_w{10};
	static const int        projectile_vertical_h{10};
	static const int        projectile_vertical_desintegration_ms{530};

	static const int        projectile_round_w{6};
	static const int        projectile_round_h{6};
	static const int        projectile_round_desintegration_ms{300};

	static const int        projectile_falling_w{6};
	static const int        projectile_falling_h{6};
	static const int        projectile_falling_desintegration_ms{300};

	private:

	d2d::collision::box     create_box(point, int, int) const;
};
}

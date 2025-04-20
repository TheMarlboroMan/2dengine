#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <d2d/components/timeouts.h>
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * The leaping monster does a leap with configurable force between rests.
 * When coming down from the leap the monster will rest on its spawn Y
 * coordinate and not in any floor. 
 */
class leaping_monster {

	public:

	enum class types {
		piranha
	};
/**
 * The second and third parameters are rest between leaps and leap force on
 * the Y exis.
 */

	                        leaping_monster(d2d::collision::point, types, int, int);

	entity                  ent;
	d2d::motion::motion_vector velocity;
	types                   type;

	void                    tic(ldtools::tdelta _delta, d2d::motion::mover);
	bool                    is_on_air() const {return on_air;}

	private:

	int                     spawn_y;
	int                     leap_force;
	d2d::components::timeout    timeout;
	bool                    on_air{false};

	static const int        piranha_w{10};
	static const int        piranha_h{14};
};

std::ostream& operator<<(std::ostream&, const leaping_monster&);
}

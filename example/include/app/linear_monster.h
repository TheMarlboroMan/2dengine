#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <iostream>

namespace app {

/**
 * the linear monster follows a straight pattern until it hits its bounds,
 * and then goes back to the other bound. These bounds are static and
 * established at construction time based on the map tiles.
 */
class linear_monster {

	public:

/**
 * represents a pair of movement bounds, the lowerst and highest this entity
 * can move on its turn.
 */
	struct boundaries {
		double  lower,
		        upper;
	};

	enum types {
		scorpion=1
	};
/**
 * the second parameter is a "type". The boolean indicates wether or not
 * this thing spaws facing right.
 */

	                        linear_monster(d2d::collision::point, int, bool, boundaries);

	entity                  ent;
	d2d::motion::motion_vector velocity;
	int                     type;
	faces                   facing;

	void                    tic(float _delta, d2d::motion::mover);

	private:

	static const int        scorpion_w{16};
	static const int        scorpion_h{16};
	static const double     scorpion_velocity;

	void                    reverse();

	boundaries              bounds;
};

std::ostream& operator<<(std::ostream&, const linear_monster&);
}

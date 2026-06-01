#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <iostream>
#include <ldtools/time_definitions.h>

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

	enum class types {
		scorpion,
		snake,
		bat,
		horizontal_bat
	};
/**
 * the second parameter is a "type". The boolean indicates wether or not
 * this thing spaws facing right.
 */

	                        linear_monster(d2d::collision::point, types, bool, boundaries);

	entity                  ent;
	types                   type;
	faces                   facing;

	void                    tic(ldtools::tdelta _delta, d2d::motion::mover);
	void                    reset();

	private:

	void                    reset_velocity();

	d2d::collision::point   starting_pos;
	app::faces              starting_face;
	boundaries              bounds;

	static const int        scorpion_w{16};
	static const int        scorpion_h{15};
	static const double     scorpion_velocity;

	static const int        snake_w{16};
	static const int        snake_h{10};
	static const double     snake_velocity;

	static const int        bat_w{11};
	static const int        bat_h{10};
	static const double     bat_velocity;

	void                    reverse();
	bool                    is_horizontal_movement() const;

};

std::ostream& operator<<(std::ostream&, const linear_monster&);
}

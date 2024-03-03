#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/motion/definitions.h>
#include <iostream>

namespace app {

/**
 * the linear monster follows a straight pattern until it hits a tile
 * and then goes back. We would establish where these tiles are as upper
 * and lower bounds in map loading time, but it would be easier for now
 * just to have the main game controller do it in each tic (so this class
 * does not even have a tic method).
 */
class linear_monster {

	public:

	enum types {
		scorpion=1
	};
/**
 * the second parameter is a "type". The boolean indicates wether or not
 * this thing spaws facing right.
 */

	                        linear_monster(d2d::collision::point, int, bool);

	entity                  ent;
	d2d::motion::motion_vector velocity;
	int                     type;
	faces                   facing;

	void                    reverse();

	static const int        scorpion_w{16};
	static const int        scorpion_h{16};
	static const double     scorpion_velocity;
};

std::ostream& operator<<(std::ostream&, const linear_monster&);
}

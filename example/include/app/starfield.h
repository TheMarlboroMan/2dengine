#pragma once

#include "app/random.h"
#include <ldtools/time_definitions.h>
#include <vector>

using ldtools::tdelta;

namespace app {

/**
 * data container for a starfield. Also includes the code that moves the 
 * stars around but nothing about drawing, that goes to the draw_game class.
 */
class starfield {

	public:

	//Velocity actually means "tics until I go one pixel up!"
	struct point{int x, y; short velocity, max_velocity;};

	//Width, height plus number of dots.
	                        starfield(int, int, int, random&);
	void                    tic(tdelta);
	void                    reload();

	const std::vector<point>& get_static() const {return static_stars;}
	const std::vector<point>& get_moving() const {return moving_stars;}

	private:

	int                     w, h;
	float                   time;
	random&                 rng;
	std::vector<point>      static_stars, moving_stars;
};
}

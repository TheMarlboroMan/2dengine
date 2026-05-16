#pragma once

#include "app/background_interface.h"
#include "app/random.h"
#include <vector>

using ldtools::tdelta;

namespace app {

/**
 * data container for a starfield. Also includes the code that moves the 
 * stars around but nothing about drawing, that goes to the draw_game class.
 */
class starfield
	:public background_interface
{
	public:

	//Velocity actually means "tics until I go one pixel up!"
	//Variant means "color variant" for moving stars.
	struct point{int x, y; short velocity, max_velocity, variant;};

	//Width, height plus number of dots.
	                        starfield(int, int, int, random&);

	//Implementation of background_interface
	void                    draw(ldv::screen&);
	void                    tic(tdelta);

	private:

	int                     w, h;
	float                   time, pulse_time;
	random&                 rng;
	std::vector<point>      static_stars, moving_stars;
};
}

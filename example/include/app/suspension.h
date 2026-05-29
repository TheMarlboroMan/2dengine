#pragma once

#include "app/background_interface.h"
#include "app/random.h"
#include "app/player_input.h"
#include <vector>

namespace app {

/**
*Stars suspended that react to player input.
*/
class suspension:
	public background_interface {

	public:

	                        suspension(const player_input&, random&, int, int, int);

	//Implementation of background_interface
	void                    draw_background(ldv::screen&);
	void                    draw_foreground(ldv::screen&);
	void                    tic(tdelta);
	int                     get_sound() const;

	private:

	struct point{
		int x, y;
		//max_dispersion=x movement range, amplitude is for y
		short dispersion, max_dispersion, amplitude, variant;
		float time_displacement;
	};

	void                    shuffle();
	void                    calculate_motion(tdelta);

	const player_input&     pli;
	random&                 rng;
	int                     w, h;
	double                  movement, time;
	std::vector<point>      points;
};
}

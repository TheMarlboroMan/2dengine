#pragma once

#include "map_transition.h"
#include <d2d/components/timeout.h>
#include <ldv/color.h>

namespace app {

/**
 *A simple "fade to black" transition.
 */
class map_transition_fade
	:public map_transition {

	public:

	enum colors {
		color_black=0,
		color_white=1
	};

	enum class directions {
		to, //0
		from //anything else.
	};

	//color, direction and duration.
	        map_transition_fade(int, int, double);

	void    tic(tdelta);
	bool    is_finished() const;
	void    draw(ldv::screen&) const;

	private:

	d2d::components::timeout    timer;
	ldv::rgba_color             color;
	directions                  direction;
};

}

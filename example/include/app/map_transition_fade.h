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

	//exit, color and direction.
	        map_transition_fade(const exit&, int, int, double);

	const app::exit get_original_exit() const {return original_exit;}
	void    tic(tdelta);
	bool    is_finished() const;
	void    draw(ldv::screen&) const;

	private:

	d2d::components::timeout    timer;
	exit                        original_exit;
	ldv::rgba_color             color;
	directions                  direction;
};

}

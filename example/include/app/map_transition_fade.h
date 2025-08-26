#pragma once

#include "map_transition.h"
#include <d2d/components/timeout.h>

namespace app {

/**
 *A simple "fade to black" transition.
 */
class map_transition_fade
	:public map_transition {

	public:

	        map_transition_fade(const exit&);

	const app::exit get_original_exit() const {return original_exit;}
	void    tic(tdelta);
	bool    is_finished() const;
	void    draw(ldv::screen&) const;

	private:

	d2d::components::timeout    timer;
	exit                        original_exit;
};

}

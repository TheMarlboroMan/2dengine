#pragma once

#include "app/background_interface.h"
#include "app/random.h"
#include <d2d/components/timeout.h>
#include <ldv/point_representation.h>
#include <vector>

using ldtools::tdelta;

namespace app {

/**
 * storm background.
 */
class storm
	:public background_interface
{
	public:

	                        storm(random&, int, int, int);

	//Implementation of background_interface
	void                    draw_background(ldv::screen&);
	void                    draw_foreground(ldv::screen&);
	void                    tic(tdelta);
	int                     get_sound() const;

	private:

	void                    tic_wait(tdelta);
	void                    tic_thunder(tdelta);
	void                    shuffle_stars();

	enum {
		timeout_wait=0,
		timeout_thunder=1
	};

	enum {
		state_wait,
		state_thunder
	};

	random&                                 rng;
	d2d::components::timeouts               timeouts;
	std::vector<ldv::point_representation>  stars,
	                                        random_stars;
	int                                     w, h, state, star_count;
	bool                                    play_sound{false};
};
}

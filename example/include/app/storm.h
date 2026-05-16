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

	                        storm(random&);

	//Implementation of background_interface
	void                    draw(ldv::screen&);
	void                    tic(tdelta);

	private:

	enum {
		timeout_wait=0,
		timeout_thunder=1
	};

	random&                                 rng;
	d2d::components::timeouts               timeouts;
	std::vector<ldv::point_representation>  stars;
};
}

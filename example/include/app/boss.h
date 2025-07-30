#pragma once

#include "entity.h"
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * game boss. Most stuff about it is hardcoded.
 */
class boss {

	public:

	                boss(d2d::collision::point, int);
	void            tic(ldtools::tdelta);

	entity          ent;

	private:

	void            stage_setup();
	void            stage_appear(ldtools::tdelta);
	void            stage_one(ldtools::tdelta);

	enum class skills {
		easy, normal, hard
	}               skill;

	enum class stages {
		setup,
		appear,
		stage_1
	}               stage{stages::setup};

	static const int w{16};
	static const int h{24};
#ifdef IS_DEBUG_BUILD
	static const int appear_y_speed{-34};
#else
	static const int appear_y_speed{-14};
#endif
	static const int horizontal_speed{34};

	static const int phase_one_x_left_limit{16};
	static const int phase_one_x_right_limit{262};

	int             appear_y_target{0};

};

std::ostream& operator<<(std::ostream&, const boss&);
}

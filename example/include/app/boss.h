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

	enum class skills {
		easy, normal, hard
	}               skill;

	static const int w{12};
	static const int h{24};

};

std::ostream& operator<<(std::ostream&, const boss&);
}

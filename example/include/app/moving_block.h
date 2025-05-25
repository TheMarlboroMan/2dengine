#pragma once

#include "entity.h"
#include <ldtools/time_definitions.h>
#include <iostream>

namespace app {

class moving_block {

	public:

	                                        moving_block(int, int, int, int, int);
	void                                    tic(ldtools::tdelta);

	entity                                  ent;


	private:

	d2d::motion::motion_vector              velocity{0.0, 0.0};
	int type;
};

std::ostream& operator<<(std::ostream&, const moving_block&);
}

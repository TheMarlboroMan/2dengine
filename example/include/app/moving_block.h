#pragma once

#include "entity.h"
#include <ldtools/time_definitions.h>
#include <iostream>

namespace app {

class moving_block {

	public:

	                                        moving_block(int, int, int, int, int);
	void                                    tic(ldtools::tdelta);
	void                                    reset();

	entity                                  ent;


	private:

	int type;
};

std::ostream& operator<<(std::ostream&, const moving_block&);
}

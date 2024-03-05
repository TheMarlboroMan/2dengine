#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * A fancy name for a black rectangle covering part of the map that must
 * be uncovered when the player walks over it.
 * Is identified by an unique id so that it can be persisted over repeated
 * visits.
 */
class secret_cover {

	public:

	                            secret_cover(d2d::collision::box, int);

	entity                      ent;
	int                         id;
	bool                        discovered{false}; //Means it must not show anymore when true.
};

std::ostream& operator<<(std::ostream&, const secret_cover&);

}

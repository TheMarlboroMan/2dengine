#pragma once

#include "definitions.h"
#include "spatiable.h"

#include <vector>

namespace d2d {namespace collision {

class checker {

	public:

	/**
	* Returns a vector of the collisions between the box and the boxes.
	*/

	//TODO: Well, this implies a copy of the spatiables into the vector...
	//and will return pointers to these copies!!!
	std::vector<spatiable const *>            get_collisions(const d2d::collision::box&, const std::vector<spatiable>&);
	std::vector<spatiable const *>            get_collisions(const d2d::collision::box&, const std::vector<spatiable const *>&);
};

}}

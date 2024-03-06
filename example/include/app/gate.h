#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * a gate that moves upwards when activated and can only be activated once.
 * When activated it moves upwards until the bottom reaches the original top
 * minus a threshold.
 */
class gate {

	public:

	                                gate(d2d::collision::box, int, int);

	entity                          ent;
	int                             id;
	int                             tag;
};

std::ostream& operator<<(std::ostream&, const gate&);
}

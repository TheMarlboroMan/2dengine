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

	                                gate(d2d::collision::box, int);

	entity                          ent;
	int                             tag;

	void                            tic(float);
	void                            activate();
/**
 * instantly opens the door.
 */
	void                            open();

	private:

	enum class states {
		closed,
		opening,
		open
	}                               state;
	int                             destination_y;

	static const int                y_threshold{4};
};

std::ostream& operator<<(std::ostream&, const gate&);
}

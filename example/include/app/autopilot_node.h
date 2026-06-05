#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * a game entity that activates/deactivates/changes the input given to the
 * autopilot when the player touches it. These are removed from the map
 * once the player touches them, to prevent weird stuff.
 */
class autopilot_node {

	public:

	                    autopilot_node(const d2d::collision::box&, int);
	void                reset();
	void                use();
	const entity&       get_entity() const {return ent;}
	int                 get_input() const {return input;}
	bool                is_used() const {return used;}

	private:

	entity                          ent;
	int                             input;
	bool                            used{false};
	friend std::ostream& operator<<(std::ostream&, const autopilot_node&);
};

std::ostream& operator<<(std::ostream&, const autopilot_node&);
}

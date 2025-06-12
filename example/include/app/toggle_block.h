#pragma once

#include "definitions.h"
#include "entity.h"
#include <ostream>

namespace app {

/**
 * A block that toggles based on player actions, jumping namely.
 */
class toggle_block {

	public:

	                        toggle_block(const d2d::collision::box&, bool, int);

	int                     get_type() const {return type;}
	bool                    is_active() const {return active;}
	void                    toggle() {active=!active;}
	void                    reset();

	entity                  ent;

	private:

	bool                    active,
	                        initial_active;
	int                     type;
};

std::ostream& operator<<(std::ostream&, const toggle_block&);
}

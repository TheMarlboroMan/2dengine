#pragma once

#include "definitions.h"
#include "entity.h"
#include <ostream>

namespace app {

/**
 * A block that is solid only when the player is facing a certain direction.
 */
class facing_block {

	public:

	                        facing_block(const d2d::collision::box&, faces, int);

	int                     get_type() const {return type;}
	bool                    is_active() const {return active;}
	bool                    is_facing_right() const {return faces::right==face;}

/**
 * activates or deactivates the block
 */
	void                    sync(faces);

	entity                  ent;

	private:

	int                     type;
	bool                    active;
	faces                   face; //face the block is active
};

std::ostream& operator<<(std::ostream&, const facing_block&);
}

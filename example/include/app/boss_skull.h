#pragma once

#include "entity.h"
#include <iostream>

namespace app {

/**
 * These are the weak points for the game boss. They just lay there and can
 * be destroyed by projectiles.
 */
class boss_skull {

	public:

	                boss_skull(d2d::collision::point);

	entity          ent;

	void            desintegrate();
	bool            is_to_be_destroyed() const {return to_be_destroyed;}

	//Public, because we need to read it from the outside in the thing loader.
	static const int w{11};
	private:

	bool            to_be_destroyed{false};
	static const int h{14};

};

std::ostream& operator<<(std::ostream&, const boss_skull&);
}

#pragma once

#include <d2d/world/collision_tile.h>

#include <vector>
#include <ostream>

namespace app { 

class map {

	public:

	void                                    clear();

	//100% public, no need to hide it.
	std::vector<d2d::world::collision_tile>             collision_tiles;

	private:

	friend std::ostream&                    operator<<(std::ostream&, const map&);
};

std::ostream& operator<<(std::ostream&, const map&);

}

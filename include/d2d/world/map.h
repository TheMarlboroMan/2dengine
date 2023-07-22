#pragma once

#include "collision_tile.h"

#include <vector>
#include <ostream>

namespace d2d { namespace world {

class map_loader;

class map {

	public:

	const std::vector<collision_tile>&      get_collision_tiles() const {return collision_tiles;}
	std::vector<collision_tile>&            get_collision_tiles() {return collision_tiles;}

	void                                    clear();

	private:

	std::vector<collision_tile>             collision_tiles;
	friend std::ostream&                    operator<<(std::ostream&, const map&);
};

std::ostream& operator<<(std::ostream&, const map&);

}}

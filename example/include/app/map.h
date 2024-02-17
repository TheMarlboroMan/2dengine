#pragma once

#include "solid_block.h"
#include "platform_block.h"
#include <d2d/collision/tile.h>
#include <vector>
#include <ostream>

namespace app { 

class map {

	public:

	void                                    clear();

	//100% public, no need to hide it.
	//TODO: Interestingly we cannot check a certain point x,y by using a grid for collisions. That's odd.
	std::vector<d2d::collision::tile>       collision_tiles;
	std::vector<solid_block>                solid_blocks;
	std::vector<platform_block>             platform_blocks;

	private:

	friend std::ostream&                    operator<<(std::ostream&, const map&);
};

std::ostream& operator<<(std::ostream&, const map&);

}

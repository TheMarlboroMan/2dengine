#pragma once

#include "solid_block.h"
#include "platform_block.h"
#include <d2d/collision/tile.h>
#include <d2d/collision/tile_finder_matrix.h>
#include <vector>
#include <ostream>

namespace app { 

class map {

	public:

	                                        map();
	void                                    clear();

	//100% public, no need to hide it.
	std::vector<d2d::collision::tile>       collision_tiles;
	std::vector<solid_block>                solid_blocks;
	std::vector<platform_block>             platform_blocks;
	d2d::collision::tile_finder_matrix      tile_finder;

/**
 * syncs the contents of collision_tiles with the tile finder.
 */
	void                                    sync_tile_finder();

	private:

	friend std::ostream&                    operator<<(std::ostream&, const map&);
};

std::ostream& operator<<(std::ostream&, const map&);

}

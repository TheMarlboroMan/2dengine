#pragma once

#include "solid_block.h"
#include "platform_block.h"
#include "ladder.h"
#include <d2d/collision/tile.h>
#include <d2d/collision/tile_finder_matrix.h>
#include <d2d/video/scenery_tile.h>
#include <ldv/color.h>
#include <vector>
#include <ostream>

namespace app { 

class map {

	public:

	                                        map();
	void                                    clear();

	//100% public, no need to hide any of these.
	std::vector<d2d::collision::tile>       collision_tiles;
	std::vector<solid_block>                solid_blocks;
	std::vector<platform_block>             platform_blocks;
	std::vector<ladder>                     ladders;
	std::vector<d2d::video::scenery_tile>   background_tiles;
	std::vector<d2d::video::scenery_tile>   foreground_tiles;
	d2d::collision::tile_finder_matrix      tile_finder;
	ldv::rgba_color                         background_color{0,0,0, 255};

/**
 * syncs the contents of collision_tiles with the tile finder.
 */
	void                                    sync_tile_finder();

	private:

	friend std::ostream&                    operator<<(std::ostream&, const map&);
};

std::ostream& operator<<(std::ostream&, const map&);

}

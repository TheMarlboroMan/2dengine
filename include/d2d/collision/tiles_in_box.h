#pragma once

#include "tile.h"
#include "spatiable.h"
#include "tile_finder.h"

#include <vector>
#include <iostream>

namespace d2d { namespace collision {

class tiles_in_box {

	public: 

	                                    tiles_in_box(int, int);

	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::spatiable&,
		const d2d::collision::tile_finder&
	) const;

	std::vector<const d2d::collision::tile *> find(
		const d2d::collision::box& _entity,
		const d2d::collision::tile_finder& _finder
	) const;

	private:

	int                 tile_w,
						tile_h;
};
}}

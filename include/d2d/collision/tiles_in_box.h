#pragma once

#include "tile.h"
#include "spatiable.h"
#include "tile_finder.h"

#ifdef IS_DEBUG_BUILD
#include <lm/logger.h>
#endif
#include <vector>
#include <iostream>

namespace d2d { namespace collision {

class tiles_in_box {

	public: 

	                    tiles_in_box(int, int);

#ifdef IS_DEBUG_BUILD

	tiles_in_box&       set_debug_enabled(bool _val) {debug_enabled=_val; return *this;}
	tiles_in_box&       set_logger(lm::logger* _val) {logger=_val; return *this;}
#endif

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
#ifdef IS_DEBUG_BUILD
	lm::logger*         logger;
	bool                debug_enabled{false};
#endif
};
}}

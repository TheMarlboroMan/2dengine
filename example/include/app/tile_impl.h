#pragma once

#include <d2d/world/collision_tile.h>
#include <d2d/world/collision_tile_implementation.h>
#include <ldv/color.h>

namespace app {

struct tile_impl:
	public d2d::world::collision_tile_implementation {
	public:

	virtual int             to_shape(const d2d::world::collision_tile& _tile) const {

		return _tile.type; //so far a 1 to 1 match.
	}

	virtual ldv::rgba_color get_outline_color(const d2d::world::collision_tile&) const {

		return ldv::rgba8(0,0,0,0);
	}

	virtual ldv::rgba_color get_fill_color(const d2d::world::collision_tile&) const {

		return ldv::rgba8(128, 128, 128, 0);
	}
};
}

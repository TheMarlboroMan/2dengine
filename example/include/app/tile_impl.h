#pragma once

#include "definitions.h"
#include <d2d/collision/tile.h>
#include <d2d/collision/tile_implementation.h>
#include <d2d/collision/shaper.h>
#include <ldv/color.h>

namespace app {

struct tile_impl:
	public d2d::collision::tile_implementation {
	public:

	virtual int             to_shape(const d2d::collision::tile& _tile) const {

		switch(_tile.type) {
			case tile_full:
			case tile_harm:
				return d2d::collision::shaper::tiles::tile_full;
			case tile_half_bottom:
			case tile_half_bottom_passable:
				return d2d::collision::shaper::tiles::tile_half_bottom;
			case tile_half_top:
			case tile_half_top_passable:
				return d2d::collision::shaper::tiles::tile_half_top;
			case tile_half_left:
				return d2d::collision::shaper::tiles::tile_half_left;
			case tile_half_right:
				return d2d::collision::shaper::tiles::tile_half_right;
			case tile_quarter_bottom_left:
				return d2d::collision::shaper::tiles::tile_quarter_bottom_left;
			case tile_quarter_bottom_right:
				return d2d::collision::shaper::tiles::tile_quarter_bottom_right;
			case tile_quarter_top_left:
				return d2d::collision::shaper::tiles::tile_quarter_top_left;
			case tile_quarter_top_right:
				return d2d::collision::shaper::tiles::tile_quarter_top_right;
		}

		throw std::runtime_error("bad tile type");
	}

	virtual ldv::rgba_color get_outline_color(const d2d::collision::tile&) const {

		return ldv::rgba8(0,0,255,255);
	}

	virtual ldv::rgba_color get_fill_color(const d2d::collision::tile&) const {

		return ldv::rgba8(128, 128, 128, 255);
	}
};
}

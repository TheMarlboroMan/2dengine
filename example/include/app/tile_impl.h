#pragma once

#include "definitions.h"
#include <d2d/collision/tile.h>
#include <d2d/collision/tile_implementation.h>
#include <d2d/collision/shaper_default.h>
#include <ldv/color.h>
#include <sstream>

namespace app {

struct tile_impl:
	public d2d::collision::tile_implementation {
	public:

	virtual int             to_shape(const d2d::collision::tile& _tile) const {

		switch(_tile.type) {
			case tile_full:
			case tile_full_no_camera:
			case tile_harm:
			case tile_monster_block:
			case tile_camera_stop:
				return d2d::collision::shaper_default::tiles::tile_full;
			case tile_half_bottom:
			case tile_half_bottom_passable:
			case tile_harm_half_bottom:
				return d2d::collision::shaper_default::tiles::tile_half_bottom;
			case tile_half_top:
			case tile_half_top_passable:
			case tile_harm_half_top:
				return d2d::collision::shaper_default::tiles::tile_half_top;
			case tile_half_left:
			case tile_harm_half_left:
				return d2d::collision::shaper_default::tiles::tile_half_left;
			case tile_half_right:
			case tile_harm_half_right:
				return d2d::collision::shaper_default::tiles::tile_half_right;
			case tile_quarter_bottom_left:
				return d2d::collision::shaper_default::tiles::tile_quarter_bottom_left;
			case tile_quarter_bottom_right:
				return d2d::collision::shaper_default::tiles::tile_quarter_bottom_right;
			case tile_quarter_top_left:
				return d2d::collision::shaper_default::tiles::tile_quarter_top_left;
			case tile_quarter_top_right:
				return d2d::collision::shaper_default::tiles::tile_quarter_top_right;
		}

		std::stringstream ss;
		ss<<"bad tile type "<<_tile.type<<" at "<<_tile.x<<","<<_tile.y;

		throw std::runtime_error(ss.str());
	}

	virtual bool is_passable_edge(
		const d2d::collision::tile& _tile,
		d2d::collision::box_edge _edge
	) const {

		switch(_tile.type) {
			case tile_full:
			case tile_harm:
			case tile_harm_half_bottom:
			case tile_harm_half_top:
			case tile_harm_half_left:
			case tile_harm_half_right:
			case tile_monster_block:
			case tile_half_bottom:
			case tile_half_top:
			case tile_half_left:
			case tile_half_right:
			case tile_quarter_bottom_left:
			case tile_quarter_bottom_right:
			case tile_quarter_top_left:
			case tile_quarter_top_right:
				return false;

			case tile_half_bottom_passable:
			case tile_half_top_passable:

				switch(_edge) {

					case d2d::collision::box_edge::top:
						return false;
					case d2d::collision::box_edge::right:
					case d2d::collision::box_edge::left:
					case d2d::collision::box_edge::bottom:
						return true;

				}
			break;

			case tile_full_no_camera:
			case tile_camera_stop:
				return true;
		}

		std::stringstream ss;
		ss<<"bad tile type "<<_tile.type<<" at "<<_tile.x<<","<<_tile.y;

		throw std::runtime_error(ss.str());
	}

	virtual ldv::rgba_color get_outline_color(const d2d::collision::tile&) const {

		return ldv::rgba8(0,0,255,255);
	}

	virtual ldv::rgba_color get_fill_color(const d2d::collision::tile&) const {

		return ldv::rgba8(128, 128, 128, 255);
	}
};
}

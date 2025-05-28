#pragma once

#include "definitions.h"
#include "d2d/collision/definitions.h"
#include "d2d/collision/tile.h"

namespace app {

/**
 * A bunch of filters...tiles that return "true" will be considered for 
 * collision. Tiles that return "false" will not.
 */

/**
 * tile filter to ignore the one way tiles above the player... These should not
 * be considered
 */
struct filter_tiles_ignore_one_way_above {

	const d2d::collision::box previous_box;

	bool operator()(
		const d2d::collision::box&,
		const d2d::collision::tile& _tile
	) const {

		if(_tile.type==app::tile_monster_block || _tile.type==app::tile_camera_stop) {

			return false;
		}

		if(_tile.type == app::tile_half_bottom_passable
			|| _tile.type == app::tile_half_top_passable
		) {

			return _tile.get_top() <= previous_box.origin.y;
		}

		return true;
	};
};

/**
 * tile filter to ignore all one way tiles.
 */
struct filter_tiles_ignore_one_way {

	bool operator()(
		const d2d::collision::box& ,
		const d2d::collision::tile& _tile
	) const {

		if(
			_tile.type==app::tile_monster_block
			|| _tile.type==app::tile_camera_stop
		) {

			return false;
		}

		return !(
			_tile.type == app::tile_half_bottom_passable
			|| _tile.type == app::tile_half_top_passable
		);
	};
};

/**
 * tile filter to extract only harm tiles.
 */
struct filter_tiles_harm_only {

	bool operator()(
		const d2d::collision::box& ,
		const d2d::collision::tile& _tile
	) const {

		return _tile.type == app::tile_harm;
	};
};

/**
 * tiles that must be ignored when checking if we are on the air.
 */
struct filter_tiles_check_on_air {

	const d2d::collision::box previous_box;

	bool operator()(
		const d2d::collision::box& _box,
		const d2d::collision::tile& _tile
	) const {

		if(
			_tile.type==app::tile_monster_block
			|| _tile.type==app::tile_camera_stop
		) {

			return false;
		}

		if(_tile.type == app::tile_harm) {

			return false;
		}

		//TODO: What???
		auto f=filter_tiles_ignore_one_way_above{previous_box};
		return f(_box, _tile);
	}
};

/**
 * tiles to be checked when projectiles move around.
 */
struct filter_tiles_projectile {

	bool operator()(
		const d2d::collision::box&,
		const d2d::collision::tile& _tile
	) const {

		return !(
			_tile.type==app::tile_half_top_passable
			|| _tile.type==app::tile_monster_block
		);
	}
};

/**
 *ignore harm tiles...
 */
struct filter_remove_harm_tiles {

	bool operator()(
		const d2d::collision::box&,
		const d2d::collision::tile& _tile
	) const {

		return _tile.type!=app::tile_harm;
	}
};
}

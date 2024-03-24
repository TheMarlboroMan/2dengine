#pragma once

#include "definitions.h"
#include "d2d/collision/definitions.h"
#include "d2d/collision/tile.h"

namespace app {

/**
 * tile filter to ignore the one way tiles above the player.
 */
struct filter_tiles_ignore_one_way_above {

	bool operator()(
		const d2d::collision::box& _box,
		const d2d::collision::tile& _tile
	) const {

		if(_tile.type==app::tile_monster_block) {

			return false;
		}

		if(_tile.type != app::tile_half_bottom_passable
			&& _tile.type != app::tile_half_top_passable
		) {

			return true;
		}

		return _tile.get_y() < _box.origin.y;
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

		if(_tile.type==app::tile_monster_block) {

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

	bool operator()(
		const d2d::collision::box& _box,
		const d2d::collision::tile& _tile
	) const {

		if(_tile.type==app::tile_monster_block) {

			return false;
		}

		if(_tile.type == app::tile_harm) {

			return false;
		}

		auto f=filter_tiles_ignore_one_way_above{};
		return f(_box, _tile);
	}
};

/**
 * tiles that must be ignored when checking if we are on the air.
 */
struct filter_tiles_ignore_monster_block {

	bool operator()(
		const d2d::collision::box&,
		const d2d::collision::tile& _tile
	) const {

		return _tile.type!=app::tile_monster_block;
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

		return _tile.type!=app::tile_monster_block;
	}
};

}

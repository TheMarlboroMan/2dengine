#pragma once

#include "moving_block.h"
#include "breaking_platform.h"
#include "facing_block.h"
#include "toggle_block.h"
#include <d2d/collision/tools.h>

namespace app {

/**
 * Filters for things, as opposed to the tile filters. Those things that return
 * true will be considered for collision, those who don't, will not.
 */

//We will also need some nice predicates and dereferencers...
struct thing_filter_breaking_platorms{

	bool operator()(const app::breaking_platform& _block) const {

		return _block.is_solid();
	}
};

struct thing_filter_facing_blocks{

	bool operator()(const app::facing_block& _block) const {

		return _block.is_active();
	}
};

struct thing_filter_toggle_blocks{

	bool operator()(const app::toggle_block& _block) const {

		return _block.is_active();
	}
};

/**
 * Moving blocks will not consider all-solid blocks unless they are 
 * below the player.
 */
struct thing_filter_moving_block {

	thing_filter_moving_block(
		const d2d::collision::box& _ref
	):ref_box(_ref) {}

	bool operator()(const app::moving_block& _block) const {

		//TODO: This will actually be "this block is fully solid".
		if(_block.get_type()==0) {

			return true;
		}

		//Now, for non solid blocks...Count only those above the reference box.
		return d2d::collision::is_above(_block.ent, ref_box);
	};

	private:
	const d2d::collision::box& ref_box;
};
}

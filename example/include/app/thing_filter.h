#pragma once

#include "moving_block.h"
#include "breaking_platform.h"
#include "facing_block.h"
#include "toggle_block.h"
#include <d2d/collision/tools.h>

//TODO:
#include <iostream>

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
 * Moving blocks will not consider semi-transparent moving blocks. These will
 * not count when a positive vector exist or they are below the player.
 * There is one exception: a block 
 */
struct thing_filter_moving_block {

	thing_filter_moving_block(
		const d2d::collision::box& _ref,
		bool _positive_y_vector
	):ref_box(_ref), positive_y_vector{_positive_y_vector} {}

	bool operator()(const app::moving_block& _block) const {

		if(_block.is_solid()) {

			return true;
		}
	
		//Non solid blocks are non-existent if there is a positive y vector.
		if(positive_y_vector) {

			return false;
		}

		//Count only those above the reference box. This takes care of
		//semi transparent blocks moving sideways towards a player and
		//not pushing them.
		return d2d::collision::is_above(_block.ent, ref_box);
	};

	private:
	const d2d::collision::box& ref_box;
	bool positive_y_vector; //meaning the _ref box (the player in this game) has a positive y vector.
};
}

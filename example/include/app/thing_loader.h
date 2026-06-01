#pragma once

#include "map.h"
#include "types.h"
#include <d2d/storage/thing_processor.h>
#include <d2d/collision/tile_limits.h>

namespace app {

class thing_loader
	: public d2d::storage::thing_processor {

	public:

	typedef std::map<std::string, d2d::storage::attribute> attrmap;

						thing_loader(map&, d2d::collision::tile_limits, tpersistence&, int);

	void                setup();
	void                load(d2d::storage::position, int, const attrmap&);

	private:

	void                add_entry(d2d::collision::point, const attrmap&);
	void                add_exit(d2d::collision::point, const attrmap&);
	void                add_ladder(d2d::collision::point, const attrmap&);
	void                add_collectible(d2d::collision::point, const attrmap&);
	void                add_linear_monster(d2d::collision::point, const attrmap&);
	void                add_leaping_monster(d2d::collision::point, const attrmap&);
	void                add_secret_cover(d2d::collision::point, const attrmap&);
	void                add_button(d2d::collision::point, const attrmap&);
	void                add_touch_trigger(d2d::collision::point, const attrmap&);
	void                add_gate(d2d::collision::point, const attrmap&);
	void                add_projectile_generator(d2d::collision::point, const attrmap&);
	void                add_breaking_platform(d2d::collision::point, const attrmap&);
	void                add_platform(d2d::collision::point, const attrmap&);
	void                add_timed_trap(d2d::collision::point, const attrmap&);
	void                add_trap(d2d::collision::point, const attrmap&);
	void                add_push_trigger(d2d::collision::point, const attrmap&);
	void                add_moving_block(d2d::collision::point, const attrmap&);
	void                add_moving_block_node(d2d::collision::point, const attrmap&);
	void                add_facing_block(d2d::collision::point, const attrmap&);
	void                add_toggle_block(d2d::collision::point, const attrmap&);
	void                add_boss(d2d::collision::point);
	void                add_boss_skull_spawn(d2d::collision::point, const attrmap&);
	void                add_text_node(const attrmap&);

	double              find_lower_x_bound(d2d::collision::tile_coords) const;
	double              find_upper_x_bound(d2d::collision::tile_coords) const;
	double              find_lower_y_bound(d2d::collision::tile_coords) const;
	double              find_upper_y_bound(d2d::collision::tile_coords) const;

	map&                curmap;
	d2d::collision::tile_limits tile_limits;
	tpersistence&       persistence;
	int                 difficulty_setting;
};
}

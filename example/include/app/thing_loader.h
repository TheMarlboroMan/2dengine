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
	void                add_secret_cover(d2d::collision::point, const attrmap&);
	void                add_button(d2d::collision::point, const attrmap&);
	void                add_gate(d2d::collision::point, const attrmap&);
	void                add_projectile_generator(d2d::collision::point, const attrmap&);

	map&                curmap;
	d2d::collision::tile_limits tile_limits;
	tpersistence&       persistence;
	int                 difficulty_setting;
};
}

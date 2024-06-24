#pragma once

#include "platform_block.h"
#include "ladder.h"
#include "entry.h"
#include "exit.h"
#include "collectible.h"
#include "linear_monster.h"
#include "leaping_monster.h"
#include "secret_cover.h"
#include "button.h"
#include "touch_trigger.h"
#include "gate.h"
#include "projectile_generator.h"
#include "projectile.h"
#include "breaking_platform.h"
#include "timed_trap.h"
#include <d2d/collision/tile.h>
#include <d2d/collision/tile_limits.h>
#include <d2d/collision/tile_finder_matrix.h>
#include <d2d/video/scenery_tile.h>
#include <ldv/color.h>
#include <vector>
#include <ostream>

namespace app { 

class map {

	public:

	                                        map();
	void                                    clear();

	//100% public, no need to hide any of these.
	std::vector<d2d::collision::tile>       collision_tiles;
	std::vector<platform_block>             platform_blocks;
	std::vector<ladder>                     ladders;
	std::vector<entry>                      entries;
	std::vector<exit>                       exits;
	std::vector<collectible>                collectibles;
	std::vector<linear_monster>             linear_monsters;
	std::vector<leaping_monster>            leaping_monsters;
	std::vector<secret_cover>               secret_covers;
	std::vector<button>                     buttons;
	std::vector<touch_trigger>              touch_triggers;
	std::vector<gate>                       gates;
	std::vector<breaking_platform>          breaking_platforms;
	std::vector<projectile_generator>       projectile_generators;
	std::vector<timed_trap>                 timed_traps;
	std::vector<d2d::video::scenery_tile>   background_tiles;
	std::vector<d2d::video::scenery_tile>   foreground_tiles;

	std::vector<app::projectile>            projectiles;
	d2d::collision::tile_finder_matrix      tile_finder;
	ldv::rgba_color                         background_color{0,0,0, 255};

/**
 * returns true if an entity is within the tile boundaries.
 */
	bool                                    is_within_boundaries(const d2d::collision::spatiable&) const;

/**
 * syncs the contents of collision_tiles with the tile finder.
 */
	void                                    sync_tile_finder();

/**
 * converts a set of tile limits to a box of world unit limits.
 */
	void                                    set_limits(d2d::collision::tile_limits);

	private:

	d2d::collision::box                     limits{{0.,0.},0,0};
};

std::ostream& operator<<(std::ostream&, const map&);

template<typename T>
void                                    print_node(const std::string _nodename, const std::vector<T>& _nodes, std::ostream& _stream) {

	_stream<<"\n\t"<<_nodename<<":["<<std::endl;

	for(const auto& node : _nodes ) {
		_stream<<node<<","<<std::endl;
	}
}

}

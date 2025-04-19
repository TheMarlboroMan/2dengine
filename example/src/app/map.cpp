#include "app/map.h"
#include "d2d/collision/tools.h"
#include "app/definitions.h"

#include <iostream>
#include <stdexcept>

using namespace app;

map::map():
	tile_finder{app::tile_w, app::tile_h}
{}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const map& _map
) {

	_stream<<"map: ["<<std::endl;
	print_node("collision_tiles", _map.collision_tiles, _stream);
	print_node("platform_blocks", _map.platform_blocks, _stream);
	print_node("ladders", _map.ladders, _stream);
	print_node("background_tiles", _map.background_tiles, _stream);
	print_node("middle_tiles", _map.middle_tiles, _stream);
	print_node("foreground_tiles", _map.foreground_tiles, _stream);
	print_node("entries", _map.entries, _stream);
	print_node("exits", _map.exits, _stream);
	print_node("collectibles", _map.exits, _stream);
	print_node("linear_monsters", _map.linear_monsters, _stream);
	print_node("leaping_monsters", _map.leaping_monsters, _stream);
	print_node("secret_covers", _map.secret_covers, _stream);
	print_node("buttons", _map.buttons, _stream);
	print_node("touch_triggers", _map.touch_triggers, _stream);
	print_node("gates", _map.gates, _stream);
	print_node("projectile_generators", _map.projectile_generators, _stream);
	print_node("breaking_platforms", _map.breaking_platforms, _stream);
	print_node("timed_traps", _map.timed_traps, _stream);
	_stream<<"]";

	return _stream;
}

void map::clear() {

	collision_tiles.clear();
	projectile_generators.clear();
	platform_blocks.clear();
	tile_finder.clear();
	ladders.clear();
	background_tiles.clear();
	middle_tiles.clear();
	foreground_tiles.clear();
	entries.clear();
	exits.clear();
	collectibles.clear();
	linear_monsters.clear();
	leaping_monsters.clear();
	secret_covers.clear();
	buttons.clear();
	touch_triggers.clear();
	gates.clear();
	breaking_platforms.clear();
	timed_traps.clear();

	projectiles.clear();
}

void map::sync_tile_finder() {

	tile_finder.clear();
	tile_finder.load(collision_tiles);
}

void map::set_limits(
	d2d::collision::tile_limits _limit
) {

	double x=_limit.left*app::tile_w;
	double y=_limit.bottom*app::tile_h;

	limits={{x, y}, _limit.w*app::tile_w, _limit.h*app::tile_h};
}

bool map::is_within_boundaries(
	const d2d::collision::spatiable& _spatiable
) const {

	return collides_with(_spatiable, limits);
}

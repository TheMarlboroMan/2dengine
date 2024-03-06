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
	print_node("solid_blocks", _map.solid_blocks, _stream);
	print_node("platform_blocks", _map.platform_blocks, _stream);
	print_node("ladders", _map.ladders, _stream);
	print_node("background_tiles", _map.background_tiles, _stream);
	print_node("foreground_tiles", _map.foreground_tiles, _stream);
	print_node("entries", _map.entries, _stream);
	print_node("exits", _map.exits, _stream);
	print_node("collectibles", _map.exits, _stream);
	print_node("linear_monsters", _map.linear_monsters, _stream);
	print_node("secret_covers", _map.secret_covers, _stream);
	print_node("buttons", _map.buttons, _stream);
	print_node("gates", _map.gates, _stream);
	_stream<<"]";

	return _stream;
}

void map::clear() {

	collision_tiles.clear();
	platform_blocks.clear();
	solid_blocks.clear();
	tile_finder.clear();
	ladders.clear();
	background_tiles.clear();
	foreground_tiles.clear();
	entries.clear();
	exits.clear();
	collectibles.clear();
	linear_monsters.clear();
	secret_covers.clear();
	buttons.clear();
	gates.clear();
}

void map::sync_tile_finder() {

	tile_finder.clear();
	tile_finder.load(collision_tiles);
}

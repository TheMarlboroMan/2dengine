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

	_stream<<"map: ["<<std::endl
		<<"\n\tcollision_tiles:["<<std::endl;
	for(const auto& tile : _map.collision_tiles ) {
		_stream<<tile<<","<<std::endl;
	}

	_stream<<"\n\tsolid_blocks:[\n";
	for(const auto& block : _map.solid_blocks) {
		_stream<<block<<","<<std::endl;
	}

	_stream<<"\n\tplatform_blocks:[\n";
	for(const auto& block : _map.platform_blocks) {
		_stream<<block<<","<<std::endl;
	}

	_stream<<"\n\tladders:[\n";
	for(const auto& ladder : _map.ladders) {
		_stream<<ladder<<","<<std::endl;
	}
	_stream<<"]";

	return _stream;
}

void map::clear() {

	collision_tiles.clear();
	platform_blocks.clear();
	solid_blocks.clear();
	tile_finder.clear();
	ladders.clear();
}

void map::sync_tile_finder() {

	tile_finder.clear();
	tile_finder.load(collision_tiles);
}

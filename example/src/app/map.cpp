#include "app/map.h"
#include "d2d/collision/tools.h"

#include <iostream>

using namespace app;

std::ostream& app::operator<<(
	std::ostream& _stream,
	const map& _map
) {

	_stream<<"map: ["<<std::endl
		<<"	collision_tiles:["<<std::endl;
	for(const auto& tile : _map.collision_tiles ) {
		_stream<<tile<<","<<std::endl;
	}

	_stream<<"]";

	return _stream;
}

void map::clear() {

	collision_tiles.clear();
}

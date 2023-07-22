#pragma once

#include "d2d/world/map.h"
#include "d2d/collision/shaper.h"

#include <rapidjson/document.h>
#include <string>

namespace d2d { namespace storage {

class map_loader {

	public:
/**
*loads data from file into map. Collision tiles in the map will be cleared.
*/
	void                                load_from_file_into_map(
	                                        const std::string&,
											d2d::world::map&,
	                                        d2d::collision::shaper&
	                                    );

	private:

	struct position {
		int x, y;
	};

	position                            parse_position(const rapidjson::Value&);
};

}}

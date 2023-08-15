#pragma once

#include "d2d/collision/shaper.h"
#include "d2d/world/collision_tile.h"
#include "d2d/world/collision_tile_implementation.h"

#include <rapidjson/document.h>
#include <string>

namespace d2d { namespace storage {

class map_loader {

	public:

/**
 * constructs the loader and stores the document into memory
 */
										map_loader(const std::string&);
/**
*loads data from file into map. Collision tiles will be cleared.
*/
	void                                load_collision_tiles(
											std::vector<d2d::world::collision_tile>&,
	                                        d2d::collision::shaper&,
											const d2d::world::collision_tile_implementation&
	                                    );

	rapidjson::Document                 doc;

	struct position {
		int x, y;
	};

	position                            parse_position(const rapidjson::Value&);
};

}}

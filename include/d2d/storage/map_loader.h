#pragma once

#include "attribute.h"
#include "definitions.h"
#include "thing_processor.h"
#include "d2d/collision/shaper.h"
#include "d2d/collision/tile.h"
#include "d2d/collision/tile_implementation.h"

#include <rapidjson/document.h>
#include <string>
#include <map>

namespace d2d { namespace storage {

class map_loader {

	public:

/**
 * constructs the loader and stores the document into memory
 */
										map_loader(const std::string&);
/**
*loads data from file into the vector. Collision tiles will be cleared.
*/
	void                                load_collision_tiles(
											std::vector<d2d::collision::tile>&,
	                                        d2d::collision::shaper&,
	                                        const d2d::collision::tile_implementation&
	                                    );

/**
*custom thing layers are processed through this method using the 
*thing_processor interface. The layer is assumed to exist.
*/
	void                                load_thing_layer(
											const std::string&,
											thing_processor&
										);

	rapidjson::Document                 doc;

	position                            parse_position(const rapidjson::Value&);

	bool                                has_layer(const std::string&, const std::string&) const;

/**
 * attempts to locate a layer by type and id. Will throw if the
 * layer does not exist
 */
	const rapidjson::Value&             locate_layer(const std::string&, const std::string&) const;

/**
 * converts a rapidjson attribute object into a map of attribute names to
 * attribute structures.
 */
	private:

	std::map<std::string, attribute> map_attributes(
		const rapidjson::Value&
	);
};


}}

#include "app/automap_game.h"

#include <stdexcept>
#include <iostream>

using namespace app;

automap_game::automap_game(
	const automap& _automap
):
	map{_automap}
{
	const auto& first=map.areas.begin();
	min_area_id=first->id;
	max_area_id=first->id;

	//Fill an area index relating indexes to area ids and other information.
	//This will last for the duration of this class.
	for(const auto& area : map.areas) {

		//Start at index zero, go up.
		area_index.insert({area.id, {area_index.size(), false}});
		max_area_id=std::max(area.id, max_area_id);
		min_area_id=std::min(area.id, min_area_id);
	}
}

void automap_game::reset() {

	for(auto& node : area_index) {

		node.second.discovered=false;
	}

	current_area_id=min_area_id;
}

const map_area& automap_game::get() const {

	return get(current_area_id);
}

const map_area& automap_game::get(
	int _area_id
) const {

	std::size_t index=area_index.at(_area_id).index;
	return map.areas.at(index);
}

void automap_game::next() {

	while(true) {

		current_area_id++;
		if(current_area_id > max_area_id) {

			current_area_id=min_area_id;
		}

#ifdef IS_DEBUG_BUILD

		if(debug_all_discovered) {

			if(!area_index.count(current_area_id)) {

				continue;
			}

			return;
		}
#endif

		if(!has(current_area_id)) {

			continue;
		}

		if(area_index.at(current_area_id).discovered) {

			return;
		}
	}
}

void automap_game::previous() {

	while(true) {

		current_area_id--;
		if(current_area_id < min_area_id) {

			current_area_id=max_area_id;
		}

#ifdef IS_DEBUG_BUILD

		if(debug_all_discovered) {

			if(!area_index.count(current_area_id)) {

				continue;
			}

			return;
		}
#endif
		if(!has(current_area_id)) {

			continue;
		}

		if(area_index.at(current_area_id).discovered) {

			return;
		}
	}
}

int automap_game::area_id_from_map(
	const std::string& _filename
) const {

	return map.find_area_by_map(_filename).id;
}

int automap_game::map_id_from_map(
	const std::string& _filename
) const {

	return map.file_to_id.at(_filename);
}

void automap_game::discover_area(
	int _area_id
) {

	area_index.at(_area_id).discovered=true;
}

void automap_game::set(
	int _id
) {

	current_area_id=_id;
}


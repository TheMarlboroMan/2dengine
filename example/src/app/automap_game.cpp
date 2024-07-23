#include "app/automap_game.h"

#include <stdexcept>
#include <iostream>

using namespace app;

automap_game::automap_game(
	const automap& _automap
):
	map{_automap}
{}

void automap_game::reset() {

	current_area_id=0;
}

const map_area& automap_game::get() const {

	auto it=std::find_if(
		std::begin(map.areas), std::end(map.areas),
		[this](const map_area& _area) -> bool {return _area.id==current_area_id;}
	);

	if(std::end(map.areas)!=it) {

		return *it;
	}

	throw std::runtime_error("unable to find area by id");
}

const map_area& automap_game::next() {

	//I guess we can add +1 to the current and check if there's something?
	//and what about wrapping??
	return map.areas.at(0);
}

const map_area& automap_game::previous() {

	return map.areas.at(0);
}

int automap_game::area_id_from_map_id(
	int _map_id
) const {

	for(const auto& area : map.areas) {

		bool has_cell=std::any_of(
			std::begin(area.cells),
			std::end(area.cells),
			[_map_id](const map_cell& _cell) -> bool {

				return _cell.id==_map_id;
			}
		);

		if(has_cell) {

			return area.id;
		}
	}

	throw std::runtime_error("unable to find area by map id");
}

void automap_game::set(
	int _id
) {

	current_area_id=_id;
}


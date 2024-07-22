#include "app/automap_game.h"

using namespace app;

automap_game::automap_game(
	const automap& _automap
):
	map{_automap},
	current_index{0}
{}

void automap_game::reset() {

}

const map_area& automap_game::get() {

	return map.areas.at(current_index);
}

const map_area& automap_game::get(
	int _area_id
) {

	current_index=_area_id;
	//TODO: Noope, by id!!!!!!
	return map.areas.at(current_index);
}

const map_area& automap_game::next() {

	//TODO: how do we know what areas are available?
	//The easy way is just to tell this class when we go into a new map,
	//right? like "make_available" or "discover_area" or whatever. This
	//would be easy to be stored in a set.
	return map.areas.at(0);
}

const map_area& automap_game::previous() {

	//TODO: how do we know what areas are available?
	return map.areas.at(0);
}

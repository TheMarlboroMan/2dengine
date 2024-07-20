#include "app/automap_game.h"

using namespace app;

automap_game::automap_game(
	const automap& _automap
):
	map{_automap},
	current_index{0},
	last_index(map.size()-1)
{}

void automap_game::reset() {

	current_index=0;
}

void automap_game::next() {

	if(current_index==last_index) {

		current_index=0;
		return;
	}

	++current_index;
}

void automap_game::previous() {

	if(current_index==0) {

		current_index=last_index;
		return;
	}

	--current_index;
}

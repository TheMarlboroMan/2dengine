#pragma once

#include "app/automap.h"

/**
 * this is the part of the game that uses the automap and interacts with
 * it, changing areas and so on.
 */

namespace app {

class automap_game {

	public:

	                automap_game(const automap&);
	void            reset();

/**
 *returns the current area.
 */
	const map_area& get();
/**
 * returns the automap area identified by the given id. Throws if it does not
 * exist.
 */
	const map_area& get(int);
/**
 * Chooses the next AVAILABLE zone.
 */
	const map_area& next();
/**
 * Chooses the previous AVAILABLE zone.
 */
	const map_area& previous();

	private:

	const automap&  map;
	std::size_t     current_index{0};
};
}

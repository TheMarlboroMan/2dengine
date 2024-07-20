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
	void            next();
	void            previous();

	private:

	const automap&  map;
	std::size_t     current_index;
	const std::size_t last_index;
};
}

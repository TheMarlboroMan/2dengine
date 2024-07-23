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
*returns an area id from the given automap id.
*/
	int   area_id_from_map_id(int) const;

/**
 *returns the current area.
 */
	const map_area& get() const;

/**
 * sets the current area id.
 */
	void  set(int);
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
	int current_area_id{0};
};
}

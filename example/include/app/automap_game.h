#pragma once

#include "app/automap.h"

#include <map>

/**
 * this is the part of the game that uses the automap and interacts with
 * it, changing areas and so on.
 */

namespace app {

class automap_game {

	public:

	                automap_game(const automap&);
	void            reset();

	int             get_min_area_id() const {return min_area_id;}
	int             get_max_area_id() const {return max_area_id;}

/**
*returns an area id from the given automap id.
*/
	int   area_id_from_map_id(int) const;

/**
 * returns true if there is an area with the given id.
 */
	bool  has(int _index) const {

		return area_index.count(_index);
	}

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
	void next();
/**
 * Chooses the previous AVAILABLE zone.
 */
	void previous();

/**
 * marks an area as discovered by id so it can be selected with next and
 * previous.
 */
	void discover_area(int);

	private:

	struct area_info {
		std::size_t index;
		bool discovered{false};
	};

	const automap&  map;
	int current_area_id{0},
	    min_area_id{0},
	    max_area_id{0};

	//map of area_id to area info.
	std::map<int, area_info>    area_index;
};
}

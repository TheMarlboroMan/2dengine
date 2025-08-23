#pragma once
/**
 * this file contains only the data structures, not the code to display and
 * discover the automap.
 */
#include <string>
#include <vector>
#include <ostream>
#include <algorithm>
#include <map>

namespace app {

struct map_area;
struct map_cell;
struct map_feature;

/**
 * the whole container of areas, containing cells, containing features.
 * plus a couple of maps from-to map filenames to map ids.
 */
struct automap {

	std::vector<map_area> areas;
	std::size_t size() const {return areas.size();}
	const map_area& find_area_by_map(const std::string&) const;
	const map_area& find_area_by_map_id(int) const;

	std::map<std::string, int>  file_to_id;
	std::map<int, std::string>  id_to_file;
};

/**
 * a group of map cells that are displayed together under a specific name.
 */
struct map_area {

	int id;
	std::string localization_key;
	bool must_draw_map;
	std::vector<map_cell> cells;
	bool has_map(int) const;
};

/**
 * a room, representing an individual map. Contains diverse features that
 * are entryways and such.
 */
struct map_cell {

	int id, x, y, w, h;
	std::vector<map_feature> features;
};

/**
 * A feature for a room. Their coordinates may point OUTSIDE the cell to 
 * allow for certain special effects.
 */
struct map_feature {

	enum flags {
		exit_top=1,
		exit_right=2, 
		exit_bottom=4, 
		exit_left=8, 
		gate=16,
		ellipsis_horizontal=32,
		ellipsis_vertical=64
	};

	int x, y, flags;
};

std::ostream& operator<<(std::ostream&, const automap&);
std::ostream& operator<<(std::ostream&, const map_area&);
std::ostream& operator<<(std::ostream&, const map_cell&);
std::ostream& operator<<(std::ostream&, const map_feature&);

}

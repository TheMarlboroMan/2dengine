#pragma once

#include "app/automap.h"
#include <string>
#include <fstream>

namespace app {

/**
 * class to produce a full map from a file. The file structure is a simple
 * text file like this:
#mark id localization_key
#list of cell id x y w h
#list of features for such a cell.
* such as
>1 map-start
*id x y w h
;x y flags
;x y flags
;x y flags
*id x y w h
;x y flags
;x y flags
*id x y w h
;x y flags
;x y flags
* we understand that the x and y coordinates represent the top-left corner of
* a box, that w goes "right" and h goes "down". There will be no comments or
* anything on the file.
*/
class automap_reader {

	public:

/**
 * reads a full map from the given file
 */
	automap     read_map(const std::string&);

	private:

	void        parse_area(automap&);
	void        parse_cell(map_area&);
	void        parse_feature(map_cell&);

	std::ifstream infile;
};
}

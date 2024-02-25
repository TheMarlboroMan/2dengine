#pragma once

#include <ostream>

namespace d2d { namespace video {

/**
 * A very simple representation of a scenery tile (foreground, background
 * or whatever. These are to be stored somewhere in a map and then drawn
 * by the scenery_tile_draw tool.
 */
struct scenery_tile {

	                scenery_tile(int, int, int);

	int             x,
	                y,
	                type; //opaque id.
};

std::ostream& operator<<(std::ostream&, const scenery_tile&);
}}

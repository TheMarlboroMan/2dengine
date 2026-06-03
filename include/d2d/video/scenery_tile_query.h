#pragma once

#include "scenery_tile_query_interface.h"
#include <string>
#include <map>

namespace d2d { namespace video {

/**
 * very basic, reusable tile to animation component. It feeds from a file
 * that contains a line per pair, first the tile index, then a space, then
 * the animation index.
 */
class scenery_tile_query
	:public d2d::video::scenery_tile_query_interface {

	public:
	//!Receives a path to a tile to animation file.
	                scenery_tile_query(const std::string&);

	virtual bool    is_animation(int) const;
	virtual int     get_animation(int) const;

	private:

	std::map<int, int> tile_to_animation;
};
}}

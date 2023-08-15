#pragma once

#include "d2d/world/map.h"
#include <ldv/camera.h>
#include <lm/logger.h>

namespace d2d { namespace video {

/**
 * given a camera and a map this class will attempt to limit the camera so
 * it will always stay inside tha map's collision limits.
 */ 
class camera_map_limit {

	public:

/**
 * these two integers are the width and height of each tile.
 */
	void            limit_to_collision_tiles(ldv::camera&, const d2d::world::map&, int, int, lm::logger* =nullptr);
};

}}

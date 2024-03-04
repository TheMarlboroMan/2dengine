#include "d2d/video/camera_map_limit.h"
#include "d2d/video/exception.h"
#include <algorithm>
#include <stdexcept>

using namespace d2d::video;

void camera_map_limit::limit_to_collision_tiles(
	ldv::camera& _camera,
	d2d::collision::tile_limits _limits,
	unsigned _w,
	unsigned _h,
	lm::logger* _logger
) {

	if(nullptr != _logger) {

		lm::log(*_logger).info()<<"camera_map_limit::limit_to_collision_tiles tile ranges are ["<<_limits.left<<" to "<<_limits.right<<"] and ["<<_limits.bottom<<" to "<<_limits.top<<"]\n";
	}

	if(!_limits) {

		if(nullptr!=_logger) {

			lm::log(*_logger).error()<<"collision tiles are empty, cannot use camera_map_limit::limit_to_collision_tiles, will throw\n";
		}

		throw exception("cannot use camera_map_limit::limit_to_collision_tiles when collision tiles has no size");
	}

	//These are cartesian system coordinates, mind you, boxes grow to the right
	//and UPWARDS, so we have to adjust accordingly...
	unsigned w=_limits.w*_w;
	unsigned h=_limits.h*_h;

	ldv::rect limit{
		_limits.left * _w,
		_limits.bottom * _h,
		w,
		h
	};

	if(nullptr != _logger) {

		lm::log(*_logger).info()<<"will set camera limits to "<<limit.origin.x<<", "<<limit.origin.y<<", "<<limit.w<<"x"<<limit.h<<"\n";
	}

	_camera.set_limits(limit);
}


#include "d2d/video/camera_map_limit.h"
#include <algorithm>
#include <stdexcept>

using namespace d2d::video;

void camera_map_limit::limit_to_collision_tiles(
	ldv::camera& _camera,
	const d2d::world::map& _map,
	int _w,
	int _h,
	lm::logger* _logger
) {

	//TODO: must throw something of this library type.
	const auto tiles=_map.get_collision_tiles();

	if(!tiles.size()) {

		if(nullptr!=_logger) {

			lm::log(*_logger).error()<<"collision tiles are empty, cannot use camera_map_limit::limit_to_collision_tiles, will throw\n";
		}

		throw std::runtime_error("cannot use camera_map_limit::limit_to_collision_tiles when collision tiles has no size");
	}

	auto x=std::minmax_element(
		std::begin(tiles),
		std::end(tiles),
		[](const d2d::world::collision_tile& _a, const d2d::world::collision_tile& _b) {
			return _a.x < _b.x;
		}
	);

	auto y=std::minmax_element(
		std::begin(tiles),
		std::end(tiles),
		[](const d2d::world::collision_tile& _a, const d2d::world::collision_tile& _b) {
			return _a.y < _b.y;
		}
	);

	int min_x=x.first->x,
		max_x=x.second->x,
		min_y=y.first->y,
		max_y=y.second->y;

	if(nullptr != _logger) {

		lm::log(*_logger).info()<<"camera_map_limit::limit_to_collision_tiles tile ranges are ["<<min_x<<" to "<<max_x<<"] and ["<<min_y<<" to "<<max_y<<"]\n";
	}

	//These are cartesian system coordinates, mind you, boxes grow to the right
	//and UPWARDS, so we have to adjust accordingly...

	int w=(max_x-min_x)*_w;
	int h=(max_y-min_y)*_h;

	ldv::rect limit{
		min_x * _w,
		min_y * _h,
		w,
		h
	};

	if(nullptr != _logger) {

		lm::log(*_logger).info()<<"will set camera limits to "<<limit.origin.x<<", "<<limit.origin.y<<", "<<limit.w<<"x"<<limit.h<<"\n";
	}

	_camera.set_limits(limit);
}


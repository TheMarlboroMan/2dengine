#include "d2d/video/tools.h"

using namespace d2d::video;
ldv::point d2d::video::to_screen_coordinates(
	ldv::point _point,
	unsigned _h
) {

	int y=-_point.y-_h;
	return {_point.x, y};
}


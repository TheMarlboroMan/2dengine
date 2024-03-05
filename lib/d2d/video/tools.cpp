#include "d2d/video/tools.h"
#include "d2d/collision/spatiable.h"

using namespace d2d::video;

ldv::point d2d::video::to_screen(
	d2d::collision::point _pt
) {

	return {(int)_pt.x, (int)_pt.y};
}

ldv::point d2d::video::to_screen_coordinates(
	ldv::point _point,
	unsigned _h
) {

	int y=-_point.y-_h;
	return {_point.x, y};
}

ldv::rect d2d::video::to_screen_rect(
	const d2d::collision::box& _box
) {
	int x=_box.origin.x;
	int y=-_box.origin.y-_box.h;
	unsigned w=_box.w;
	unsigned h=_box.h;
	return {{x, y}, w, h};
}

ldv::rect d2d::video::to_screen_rect(
	const d2d::collision::spatiable& _spatiable
) {

	return to_screen_rect(_spatiable.get_box());
}

#include "d2d/collision/spatiable.h"

using namespace d2d::collision;

spatiable::~spatiable() {

}

void spatiable::set_box(
	box _box
) {

	auto& b=get_box();
	b=_box;
}

void spatiable::set_origin(
	point _pt
) {

	auto& b=get_box();
	b.origin=_pt;
}

void spatiable::set_x(
	double _val
) {

	auto& b=get_box();
	b.origin.x=_val;
}

void spatiable::set_y(
	double _val
) {

	auto& b=get_box();
	b.origin.y=_val;
}

void spatiable::set_w(
	int _val
) {

	auto& b=get_box();
	b.w=_val;
}

void spatiable::set_h(
	int _val
) {

	auto& b=get_box();
	b.h=_val;
}

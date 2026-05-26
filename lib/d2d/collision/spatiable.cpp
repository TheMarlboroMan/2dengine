#include "d2d/collision/spatiable.h"

using namespace d2d::collision;

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream, 
	const box_edge& _edge
) {

	switch(_edge) {

		case box_edge::top:     _stream<<"box_edge::top"; break;
		case box_edge::right:   _stream<<"box_edge::right"; break;
		case box_edge::bottom:  _stream<<"box_edge::bottom"; break;
		case box_edge::left:    _stream<<"box_edge::left"; break;
	}

	return _stream;
}

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

void spatiable::move_by(
	point _pt
) {

	auto& b=get_box();
	b.origin+=_pt;
}

void spatiable::set_x(
	t_position _val
) {

	auto& b=get_box();
	b.origin.x=_val;
}

void spatiable::set_y(
	t_position _val
) {

	auto& b=get_box();
	b.origin.y=_val;
}

void spatiable::set_w(
	t_size _val
) {

	auto& b=get_box();
	b.w=_val;
}

void spatiable::set_h(
	t_size _val
) {

	auto& b=get_box();
	b.h=_val;
}

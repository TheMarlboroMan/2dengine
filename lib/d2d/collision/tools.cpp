#include "d2d/collision/tools.h"
#include <iostream>

using namespace d2d::collision;

bool d2d::collision::is_above(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_above(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_above(
	const spatiable& _a, 
	const box& _b) {

	return is_above(_a.get_box(), _b);
}

bool d2d::collision::is_above(
	const box& _a, 
	const spatiable& _b) {

	return is_above(_a, _b.get_box());
}

bool d2d::collision::is_above(
	const box& _a, 
	const box& _b) {

	return _a.origin.y >= _b.origin.y+_b.h;
}

bool d2d::collision::is_below(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_below(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_below(
	const spatiable& _a, 
	const box& _b
) {
	
	return is_below(_a.get_box(), _b);
}

bool d2d::collision::is_below(
	const box& _a, 
	const spatiable& _b
) {
	
	return is_below(_a, _b.get_box());
}

bool d2d::collision::is_below(
	const box& _a, 
	const box& _b
) {

	return _a.origin.y+_a.h <= _b.origin.y;
}

bool d2d::collision::is_left_of(
	const spatiable& _a,
	const spatiable& _b
) {

	return is_left_of(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_left_of(
	const spatiable& _a, 
	const box& _b
) {

	return is_left_of(_a.get_box(), _b);
}

bool d2d::collision::is_left_of(
	const box& _a, 
	const spatiable& _b
) {

	return is_left_of(_a, _b.get_box());
}

bool d2d::collision::is_left_of(
	const box& _a, 
	const box& _b
) {

	return _a.origin.x+_a.w <= _b.origin.x;
}

bool d2d::collision::is_right_of(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_right_of(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_right_of(
	const spatiable& _a, 
	const box& _b
) {

	return is_right_of(_a.get_box(), _b);
}

bool d2d::collision::is_right_of(
	const box& _a, 
	const spatiable& _b
) {

	return is_right_of(_a, _b.get_box());
}

bool d2d::collision::is_right_of(
	const box& _a, 
	const box& _b
) {

	return _a.origin.x >= _b.origin.x + _b.w;
}

bool d2d::collision::collides_with(
	const spatiable& _a, 
	const spatiable& _b,
	bool _touch_is_collision
) {

	return _a.get_box().collides_with(_b.get_box(), _touch_is_collision);
}

bool d2d::collision::collides_with(
	const spatiable& _a, 
	const box& _b,
	bool _touch_is_collision
) {

	return _a.get_box().collides_with(_b, _touch_is_collision);
}

bool d2d::collision::collides_with(
	const box& _a, 
	const spatiable& _b,
	bool _touch_is_collision
) {

	return _a.collides_with(_b.get_box(), _touch_is_collision);
}

bool d2d::collision::collides_with(
	const box& _a, 
	const box& _b,
	bool _touch_is_collision
) {

	return _a.collides_with(_b, _touch_is_collision);
}

void d2d::collision::snap_to_right_of(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().snap_to_edge(_b.get_box(), ldt::box_edges::left);
}

void d2d::collision::snap_to_left_of(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().snap_to_edge(_b.get_box(), ldt::box_edges::right);
}

void d2d::collision::snap_to_top_of(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().snap_to_edge(_b.get_box(), ldt::box_edges::bottom);
}

void d2d::collision::snap_to_bottom_of(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().snap_to_edge(_b.get_box(), ldt::box_edges::top);
}

void d2d::collision::match_right_of(
	spatiable& _a, 
	const spatiable& _b,
	double _margin
) {


	_a.get_box().match_edge(_b.get_box(), ldt::box_edges::right);
	if(_margin) {

		_a.set_x(_a.get_x()-_margin);
	}
}

void d2d::collision::match_left_of(
	spatiable& _a,
	const spatiable& _b,
	double _margin
) {

	_a.get_box().match_edge(_b.get_box(), ldt::box_edges::left);
	if(_margin) {

		_a.set_x(_a.get_x()+_margin);
	}
}

void d2d::collision::match_top_of(
	spatiable& _a, 
	const spatiable& _b,
	double _margin
) {

	_a.get_box().match_edge(_b.get_box(), ldt::box_edges::top);
	if(_margin) {

		_a.set_y(_a.get_y()-_margin);
	}
}

void d2d::collision::match_bottom_of(
	spatiable& _a, 
	const spatiable& _b,
	double _margin
) {

	_a.get_box().match_edge(_b.get_box(), ldt::box_edges::bottom);
	if(_margin) {

		_a.set_y(_a.get_y()+_margin);
	}
}

void d2d::collision::center_horizontally(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().center_horizontally(_b.get_box());
}

void d2d::collision::center_horizontally(
	spatiable& _a, 
	const box& _b
) {

	_a.get_box().center_horizontally(_b);
}

void d2d::collision::center_horizontally(
	box& _a, 
	const box& _b
) {

	_a.center_horizontally(_b);
}

void d2d::collision::center_vertically(
	spatiable& _a, 
	const spatiable& _b
) {

	_a.get_box().center_vertically(_b.get_box());
}

void d2d::collision::center_vertically(
	spatiable& _a, 
	const box& _b
) {

	_a.get_box().center_vertically(_b);
}

void d2d::collision::center_vertically(
	box& _a, 
	const box& _b
) {

	_a.center_vertically(_b);
}

bool d2d::collision::is_partially_above(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_partially_above(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_partially_above(
	const spatiable& _a, 
	const box& _b) {

	return is_partially_above(_a.get_box(), _b);
}

bool d2d::collision::is_partially_above(
	const box& _a, 
	const spatiable& _b) {

	return is_partially_above(_a, _b.get_box());
}

bool d2d::collision::is_partially_above(
	const box& _a, 
	const box& _b
) {

	const auto b_top=_b.origin.y+_b.h;

	return _a.origin.y+_a.h > b_top
		&& _a.origin.y < b_top; 
}

bool d2d::collision::is_partially_below(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_partially_below(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_partially_below(
	const spatiable& _a, 
	const box& _b
) {
	
	return is_partially_below(_a.get_box(), _b);
}

bool d2d::collision::is_partially_below(
	const box& _a, 
	const spatiable& _b
) {
	
	return is_partially_below(_a, _b.get_box());
}

bool d2d::collision::is_partially_below(
	const box& _a, 
	const box& _b
) {

	return _a.origin.y < _b.origin.y
		&& _a.origin.y+_a.h > _b.origin.y;
}

bool d2d::collision::is_partially_left_of(
	const spatiable& _a,
	const spatiable& _b
) {

	return is_partially_left_of(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_partially_left_of(
	const spatiable& _a, 
	const box& _b
) {

	return is_partially_left_of(_a.get_box(), _b);
}

bool d2d::collision::is_partially_left_of(
	const box& _a, 
	const spatiable& _b
) {

	return is_partially_left_of(_a, _b.get_box());
}

bool d2d::collision::is_partially_left_of(
	const box& _a, 
	const box& _b
) {

	return _a.origin.x < _b.origin.x
		&& _a.origin.x+_a.w > _b.origin.x;
}

bool d2d::collision::is_partially_right_of(
	const spatiable& _a, 
	const spatiable& _b
) {

	return is_partially_right_of(_a.get_box(), _b.get_box());
}

bool d2d::collision::is_partially_right_of(
	const spatiable& _a, 
	const box& _b
) {

	return is_partially_right_of(_a.get_box(), _b);
}

bool d2d::collision::is_partially_right_of(
	const box& _a, 
	const spatiable& _b
) {

	return is_partially_right_of(_a, _b.get_box());
}

bool d2d::collision::is_partially_right_of(
	const box& _a, 
	const box& _b
) {

	const auto b_right=_b.origin.x+_b.w;

	return _a.origin.x+_a.w > b_right
		&& _a.origin.x < b_right;
}

bool d2d::collision::is_compromised_edge(
	int _edges
) {

	return ((_edges & top) && (_edges & bottom))
		|| ((_edges & left) && (_edges & right));
}



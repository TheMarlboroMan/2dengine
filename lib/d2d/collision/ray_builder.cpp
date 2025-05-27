#include "d2d/collision/ray_builder.h"

using namespace d2d::collision;

d2d::collision::ray ray_builder::get(
	const spatiable& _target,
	const d2d::motion::motion_vector& _vector
) const {

	return {
		{
			_target.get_x()+(_target.get_w() / 2.),
			_target.get_y()+(_target.get_h() / 2.)
		},
		_vector
	};
}

d2d::collision::ray ray_builder::get_previous(
	const spatiable& _target,
	const d2d::motion::motion_vector& _vector
) const {

	const auto& box=_target.get_previous_box();

	return {
		{
			box.origin.x+(box.w / 2.),
			box.origin.y+(box.h / 2.)
		},
		_vector
	};
}

d2d::collision::ray ray_builder::get(
	const spatiable& _target
) const {

	return {
		{
			_target.get_x()+(_target.get_w() / 2.),
			_target.get_y()+(_target.get_h() / 2.)
		},
		_target.get_motion_vector()
	};
}

d2d::collision::ray ray_builder::get_previous(
	const spatiable& _target
) const {

	const auto& box=_target.get_previous_box();

	return {
		{
			box.origin.x+(box.w / 2.),
			box.origin.y+(box.h / 2.)
		},
		_target.get_motion_vector()
	};
}


#include "d2d/collision/ray_builder.h"

using namespace d2d::collision;

d2d::collision::ray ray_builder::get(
	const spatiable& _target,
	const d2d::motion::motion_vector& _vector
) const {

	constexpr t_position half=(t_position) 2;

	return {
		{
			_target.get_x()+(_target.get_w() / half),
			_target.get_y()+(_target.get_h() / half)
		},
		_vector
	};
}

d2d::collision::ray ray_builder::get_previous(
	const spatiable& _target,
	const d2d::motion::motion_vector& _vector
) const {

	const auto& box=_target.get_previous_box();
	constexpr t_position half=(t_position) 2;

	return {
		{
			box.origin.x+(box.w / half),
			box.origin.y+(box.h / half)
		},
		_vector
	};
}

d2d::collision::ray ray_builder::get(
	const spatiable& _target
) const {

	constexpr t_position half=(t_position) 2;

	return {
		{
			_target.get_x()+(_target.get_w() / half),
			_target.get_y()+(_target.get_h() / half)
		},
		_target.get_motion_vector()
	};
}

d2d::collision::ray ray_builder::get_previous(
	const spatiable& _target
) const {

	const auto& box=_target.get_previous_box();
	constexpr t_position half=(t_position) 2;

	return {
		{
			box.origin.x+(box.w / half),
			box.origin.y+(box.h / half)
		},
		_target.get_motion_vector()
	};
}


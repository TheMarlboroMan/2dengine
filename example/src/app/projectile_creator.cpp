#include "app/projectile_creator.h"
#include <cmath>
#include <ldt/tools.h>

using namespace app;

projectile projectile_creator::create_horizontal(
	point _origin, 
	double _velocity
) const {

	auto box=create_box(_origin, projectile_horizontal_w, projectile_horizontal_h);

	return {
		box,
		{_velocity, 0.0},
		projectile::types::horizontal
	};
}

projectile projectile_creator::create_vertical(
	point _origin, 
	double _velocity
) const {

	auto box=create_box(_origin, projectile_vertical_w, projectile_vertical_h);

	return {
		box,
		{0.0, _velocity},
		projectile::types::vertical
	};
}

projectile projectile_creator::create_falling(
	point _origin, 
	double _velocity
) const {

	auto box=create_box(_origin, projectile_falling_w, projectile_falling_h);

	return {
		box,
		{0.0, -fabs(_velocity)},
		projectile::types::falling
	};
}

projectile projectile_creator::create_targeted(
	point _origin, 
	double _velocity, 
	point _target,
	int _extra_angle
) const {

	auto angle_rad=ldt::angle_between_rad(_origin, _target);
	if(0!=_extra_angle) {

		angle_rad+=ldt::deg_to_rad((double)_extra_angle);
	}

	d2d::motion::motion_vector vec{ldt::vector_from_angle_rad(angle_rad)};
	vec.normalize();
	vec*=fabs(_velocity);

	auto box=create_box(_origin, projectile_round_w, projectile_round_h);

	return {
		box,
		vec,
		projectile::types::directed
	};
}

projectile projectile_creator::create_directed(
	point _origin, 
	double _velocity,
	int _angle
) const {

	d2d::motion::motion_vector vec{ldt::vector_from_angle((d2d::motion::t_motion)_angle)};
	vec.normalize();
	vec*=fabs(_velocity);

	auto box=create_box(_origin, projectile_round_w, projectile_round_h);

	return {
		box,
		vec,
		projectile::types::directed
	};
}

d2d::collision::box projectile_creator::create_box(
	point _origin, 
	int _w,
	int _h
) const {

	_origin.x-=_w/2;
	_origin.y-=_h/2;
	return {_origin, _w, _h};
}


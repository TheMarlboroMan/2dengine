#include "d2d/collision/ray_aabb_finder.h"
#include <ldt/aabb_ray_collision.h>

#ifdef IS_DEBUG_BUILD
	#include <iostream>
#endif

using namespace d2d::collision;

d2d::collision::ray_aabb_info ray_aabb_finder::find(
	const ray& _ray,
	const collision::spatiable& _target,
	const collision::spatiable& _obstacle
) const {

	double half_w=_target.get_w()/2.0,
	    half_h=_target.get_h()/2.0;

	ray r_copy=_ray;
	const auto& before=_target.get_previous_box();
	r_copy.point=before.origin;
	r_copy.point.x+=half_w;
	r_copy.point.y+=half_h;

	return find_with_previous_ray(r_copy, _obstacle, half_w, half_h);
}

d2d::collision::ray_aabb_info ray_aabb_finder::find_with_previous_ray(
	const ray& _ray,
	const collision::spatiable& _obstacle,
	double _half_w,
	double _half_h
) const {

	//See the ray_aabb_checker to know what's going on here.
	const auto& obstacle=_obstacle.get_box();
	ldt::box<double, double> enlarged={
		obstacle.origin,
		(double)obstacle.w+(_half_w * 2),
		(double)obstacle.h+(_half_h * 2)
	};

	enlarged.origin.x-=_half_w;
	enlarged.origin.y-=_half_h;

	auto intersection=ray_intersects_box(_ray, enlarged);

	//Notice that we are not returning the intersection point because this is
	//supposed to be box vs box: the collision occurs alongside a full side!

	return ray_aabb_info{
		intersection.has_intersection,
		intersection.intersection_t,
		intersection.normal,
		&_obstacle
	};
}

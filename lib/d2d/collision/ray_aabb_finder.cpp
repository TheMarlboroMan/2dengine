#include "d2d/collision/ray_aabb_finder.h"
#include <ldt/aabb_ray_collision.h>

using namespace d2d::collision;

d2d::collision::ray_aabb_info ray_aabb_finder::find(
	const ray& _ray,
	const collision::spatiable& _target,
	const collision::spatiable& _obstacle
) const {

	//See the ray_aabb_checker to know what's going on here.
	const auto& obstacle=_obstacle.get_box();
	ldt::box<t_position, t_position> enlarged={
		obstacle.origin,
		(t_position)obstacle.w+_target.get_w(),
		(t_position)obstacle.h+_target.get_h()
	};

	t_position half_w=_target.get_w()/2.0,
	    half_h=_target.get_h()/2.0;

	enlarged.origin.x-=half_w;
	enlarged.origin.y-=half_h;

	auto intersection=ldt::ray_intersects_box(_ray, enlarged);

	//Notice that we are not returning the intersection point because this is
	//supposed to be box vs box: the collision occurs alongside a full side!

	return ray_aabb_info{
		intersection.has_intersection,
		intersection.intersection_t,
		intersection.normal,
		&_obstacle
	};
}


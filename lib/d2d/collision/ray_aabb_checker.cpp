#include "d2d/collision/ray_aabb_checker.h"
#include <ldt/aabb_ray_collision.h>

using namespace d2d::collision;

bool ray_aabb_checker::check(
	const ray& _ray,
	const spatiable& _box,
	const spatiable& _obstacle
) const {

	//First off, the ray can be so small as to not go outside of its box (box)
	//so here we have two options: extend it until it touches the box (and start
	//there) or lie and expand the obstacle. We will do this last one.
	
	const auto& obstacle=_obstacle.get_box();
	double half_w=(double)_box.get_w()/2.,
	    half_h=(double)_box.get_h()/2.;

	//Ok... the original box has an integer as its "size" but we need more
	//precision so let's make up for it... I can see how this is going to
	//be a problem in many cases.
	ldt::box<double, double> enlarged={
		obstacle.origin,
		(double)obstacle.w,
		(double)obstacle.h
	};

	enlarged.origin.x-=half_w;
	enlarged.origin.y-=half_h;
	enlarged.w+=half_w*2.;
	enlarged.h+=half_h*2.;

	//More suffering... the idea of rays vs aabb is "predictive" in nature and
	//we want to work in a "corrective" way so we really need to set the ray
	//in the position of the previous tic, to check if, from there, we have
	//jumped into a collision now!.
	
	ray r_copy=_ray;
	const auto& before=_box.get_previous_box();
	r_copy.point=before.origin;
	r_copy.point.x+=half_w;
	r_copy.point.y+=half_h;

	return ldt::ray_intersects_box_simple(r_copy, enlarged);
}

#pragma once

#include "definitions.h"
#include "spatiable.h"
#include <ldt/aabb_ray_collision.h>

#include <vector>

namespace d2d { namespace collision {

/**
 * information about a potential collision between a ray and an spatiable.
 * That one of these is produced does not mean that a collision took place,
 * for that we have to resort to has_intersection or the boolean operator.
 * The intersection time is the moment on the ray (between 0 and 1) in which
 * the nearest intersection with the aabb box of "obstacle" takes place. 
 * There will always be a reference to the "obstacle", it being the thing
 * the ray is being checked against. It's just a pointer so we can have copy
 * it around.
 */
struct ray_aabb_info {

	bool has_intersection;
	double intersection_time;
	ldt::vector_2d<double> normal;
	const spatiable * obstacle{nullptr};
	operator bool() const {return has_intersection;}
	bool operator < (const ray_aabb_info& _other) const {

		return intersection_time < _other.intersection_time;
	}
};

/**
 * Attempts to report the status of a collision between spatiables using 
 * a ray vs aabb method, that is, assumes the target spatiable is moving and
 * is also correctly implementing the spatiable interface by providing a 
 * "previous box". 
 * The ray method itself is "predictive" (reports if a collision will happen
 * from a point when tracing a ray into the future) so the algorithms used
 * will attempt some black magic to work in a "corrective" way.
*/
class ray_aabb_finder {

	public:

/**
 * produces a response for a single collision but does not assume that a 
 * collision has taken place (the response can be used as a bool for that
 * purpose). The ray is assumed to be the movement that took place in this
 * current tic, it is also assumed that the first spatiable box is its current
 * position (where a collision may be taking place) and its previous box 
 * represents the previous tic (where no collisions took place).
 */
	ray_aabb_info find(const ray&, const collision::spatiable&, const collision::spatiable&) const;

/**
 * Templated version for vectors of elements that can behave as spatiables.
 * Returns a vector of collisions that really took place.
 */
	template<typename T>
	std::vector<ray_aabb_info> find(
		const ray& _ray,
		const collision::spatiable& _target,
		const std::vector<T>& _obstacles
	) const {

		double half_w=_target.get_w()/2.0,
	        half_h=_target.get_h()/2.0;

		ray r_copy=_ray;
		const auto& before=_target.get_previous_box();
		r_copy.point=before.origin;
		r_copy.point.x+=half_w;
		r_copy.point.y+=half_h;

		std::vector<ray_aabb_info> result;
		for(const auto& node : _obstacles) {

			auto info=this->find_with_previous_ray(r_copy, node, half_w, half_h);
			if(info) {

				result.push_back(std::move(info));
			}
		}

		return result;
	}

	private:

	/**
	* saves some repetitive calculations.
	*/
	ray_aabb_info find_with_previous_ray(
		const ray&, 
		const collision::spatiable&, 
		double,
		double
	) const;

};

}}

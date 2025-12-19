#include "d2d/collision/ray_aabb_phase.h"

#ifdef IS_DEBUG_BUILD
	#include <iostream>
#endif

using namespace d2d::collision;

ray_aabb_phase::ray_aabb_phase(
	d2d::collision::spatiable& _subject,
	const d2d::collision::ray& _ray
):
	subject{_subject},
	ray{_ray}
{}

ray_aabb_phase& ray_aabb_phase::reset() {

	results.clear();
	return *this;
}

ray_aabb_phase& ray_aabb_phase::reset_modifiers() {

	collision_flags=0;
	with_early_exit=false;
	return *this;
}

ray_aabb_phase& ray_aabb_phase::flags(
	int _val
) {
	collision_flags=_val;
	return *this;
}

ray_aabb_phase& ray_aabb_phase::early_exit(
	bool _val
) {

	with_early_exit=_val;
	return *this;
}

ray_aabb_phase& ray_aabb_phase::detect_one(
	const d2d::collision::spatiable& _node, 
	int _flags
) {

	auto info=finder.find(ray, subject, _node);
	if(!info) {

		return *this;
	}

	int flags=-1==_flags 
		? collision_flags
		: 0;

	bool must_check_side=!(flags & flag_skip_passable_side_check);
	if(must_check_side) {

		if(info.normal.x!=0.) {

			auto edge=info.normal.x < 0.
				? box_edge::left
				: box_edge::right;

			if(_node.is_passable_edge(edge)) {

				return *this;
			}
		}

		if(info.normal.y != 0.) {

			auto edge=info.normal.y < 0.
				? box_edge::bottom
				: box_edge::top;

			if(_node.is_passable_edge(edge)) {

				return *this;
			}
        }
	}

	collision_found=true;
	results.push_back(info);

	return *this;
}

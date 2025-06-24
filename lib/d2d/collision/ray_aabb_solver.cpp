#include "d2d/collision/ray_aabb_solver.h"
#include "d2d/collision/tools.h"
#include <stdexcept>

#ifdef IS_DEBUG_BUILD
	#include <iostream>
#endif

using namespace d2d::collision;

ray_aabb_solver::result ray_aabb_solver::sort_and_solve(
	d2d::collision::spatiable& _target,
	std::vector<ray_aabb_info>& _info
) const {
	
	std::sort(std::begin(_info), std::end(_info));
	return solve(_target, _info);
}

//TODO: Maybe we can pass options, such as FILL_OUT_WHICH
ray_aabb_solver::result ray_aabb_solver::solve(
	d2d::collision::spatiable& _target,
	const std::vector<ray_aabb_info>& _info
) const {

	int edges=0, collision_count=0;
	for(const auto& info : _info) {

		//What if we moved out of collision by then????? Just skip, right??
		if(!collides_with(_target, *info.obstacle)) {

			continue;
		}

		++collision_count;

		int current_edge=0;
		if(info.normal.x) {

			current_edge=info.normal.x < 0 ? aabb_edges::left : aabb_edges::right;
		}
		else if(info.normal.y) {

			current_edge=info.normal.y < 0 ? aabb_edges::bottom : aabb_edges::top;
		}
		else {

			throw std::runtime_error("not x or y normal!");
		}

		switch(current_edge) {
			case top: d2d::collision::snap_to_top_of(_target, *info.obstacle); break;
			case bottom: d2d::collision::snap_to_bottom_of(_target, *info.obstacle); break;
			case left: d2d::collision::snap_to_left_of(_target, *info.obstacle); break;
			case right: d2d::collision::snap_to_right_of(_target, *info.obstacle); break;
		}

		edges|=current_edge;
	}

	return {edges, collision_count};
}


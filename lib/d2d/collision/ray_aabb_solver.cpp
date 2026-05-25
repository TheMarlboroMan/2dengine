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

	double x_edge=0.;
	double y_edge=0.;
	int edges=0, collision_count=0;
	for(const auto& info : _info) {

		//What if we moved out of collision by then????? Just skip, right??
		//TODO: The bug is... here...we are using a broad phase again or whatever.
		//At this point I am grasping at straws here...  I cannot replace this
		//because I don't have the ray anymore. Fuck fuck fuck fuck.
		//TODO: You know, maybe... a ray told us there was a collision so
		//let's assume touch is collision? that should still work!
		//TODO:
		//Mixing rays and aabb is a recipe for disaster!
		/*
		if(!collides_with(_target, *info.obstacle, false)) {

			//TODO: Are you the one with the bug??????
			continue;
		}
		*/


		if(edges & aabb_edges::left) {

			if(info.obstacle->get_x() >= x_edge) {

				continue;
			}
		}
		else if(edges & aabb_edges::right) {

			if(info.obstacle->get_right() <= x_edge) {

				continue;
			}
		}

		if(edges & aabb_edges::bottom) {

			if(info.obstacle->get_y() >= y_edge) {

				continue;
			}
		}
		else if(edges & aabb_edges::top) {

			if(info.obstacle->get_top() <= y_edge) {

				continue;
			}
		}

		int current_edge=0;
		++collision_count;

		if(info.normal.x) {

			x_edge=info.normal.x < 0 ? info.obstacle->get_x() : info.obstacle->get_right();
			current_edge=info.normal.x < 0 ? aabb_edges::left : aabb_edges::right;
		}
		else if(info.normal.y) {

			y_edge=info.normal.y < 0 ? info.obstacle->get_y() : info.obstacle->get_top();
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


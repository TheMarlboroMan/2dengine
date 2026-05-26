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

		//There was a terrible bug here that I am compelled to document, so
		//as not do do it again. For each obstacle we are potentially changing
		//the subject position by snapping. which means that further obstacles
		//may not be in collision anymore, thus we need to check if the updated
		//position still needs a correction. 
		//The collisions came from a ray, and we were using good old segment
		//overlapping to check for collisions: this is mixing two worlds and
		//was causing a rare precision bug in which the ray said there was
		//a collision by the segment collision did not (because 244.000000000000003
		//+12 is represented exactly as 256!), thus skipping all corrections
		//and causing the player to eventually enter the wall until the 
		//segment collision detected it and killed the player at the end of the
		//tic.
		//What we are doing now is simple: if we get a correction in an edge,
		//all obstacles further away or in the same position of that edge are
		//ignored. This way we correct at least one collision reported by the
		//ray, reposition the player and respect the separation collision check
		//types (using only rays in this case).

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


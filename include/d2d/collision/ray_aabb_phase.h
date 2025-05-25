#pragma once

#include "ray_aabb_finder.h"
#include "../tools/to_ref.h"

namespace d2d { namespace collision {

/**
 * convenience class to treat collisions from different types as a single
 * phase by adding them. See the documentation of aabb phase to understand the
 * meaning of each method as it follows roughly the same public interface.
 * unlike the aabb_phase there is no vertical or horizontal phase: the whole 
 * thing is done as a whole.
 */
class ray_aabb_phase {

	public:

	//TODO: This is repeated in the ray_phase...
	enum flags{
		flag_none=0,
		flag_skip_passable_side_check=1 //the box must be considered non-passable on all sides.
	};

	                    ray_aabb_phase(
	                        d2d::collision::spatiable&,
	                        const d2d::collision::ray&
						);

	std::size_t         size() const {return results.size();}
	bool                has_collision() const {return collision_found;}
	ray_aabb_phase&     reset_modifiers();
	ray_aabb_phase&     reset();
	ray_aabb_phase&     flags(int);
	ray_aabb_phase&     early_exit(bool);
	ray_aabb_phase&     detect_one(const d2d::collision::spatiable&, int=0);
	ray_aabb_phase&     detect_one(const d2d::collision::spatiable * _node, int _flags=0) {return detect_one(*_node, _flags);}

	template<typename T>
	ray_aabb_phase&     detect_all(
		T& _nodes
	) {

		for(const auto& node : _nodes) {

			detect_one(d2d::tools::to_ref(node), collision_flags);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

	template<typename T, typename D>
	ray_aabb_phase&     detect_all(
		T& _nodes,
		const D& _dereferencer
	) {

		for(const auto& node : _nodes) {

			const auto& spatiable=_dereferencer(node);
			detect_one(d2d::tools::to_ref(spatiable), collision_flags);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

	template<typename T, typename P>
	ray_aabb_phase&       detect_if(
		T& _nodes,
		const P& _skipper
	) {

		for(const auto& node : _nodes) {

			const auto& ref=d2d::tools::to_ref(node);
			if(!_skipper(ref)) {

				continue;
			}

			detect_one(ref, collision_flags);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

/**
 * invokes a generic response that will adjust position according to the 
 * aabb_solver algorithms.
 */
	int                             response_generic();

	private:

	bool                            collision_found{false},
	                                with_early_exit{false};
	int                             collision_flags{0};
	d2d::collision::spatiable&      subject;
	const d2d::collision::ray&      ray;
	d2d::collision::ray_aabb_finder finder;
	std::vector<ray_aabb_info>      results;
};

}}

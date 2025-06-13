#pragma once
#include "spatiable.h"
#include "tools.h"

#include <vector>

namespace d2d {namespace collision {

/**
*Collision checker whose main purpose is to maintain the types of the 
*obstacles we collide with!
*/
class aabb_checker {

	public:

	enum flags{
		flag_none=0,
		flag_skip_passable_side_check=1 //the box must be considered non-passable on all sides.
	};

	                        aabb_checker(const d2d::collision::spatiable&);
	                        aabb_checker(const d2d::collision::box&);

/**
 * resets the subject
 */

	aabb_checker&           set_subject(const d2d::collision::spatiable&);
	aabb_checker&           set_subject(const d2d::collision::box&);


/**
 * generic get_collisions version that will return a vector of pointers to 
 * targets preserving their type. Does not take into account collision phase
 * at all, just checks if the nodes intersect with the target. The target
 * type might be an spatiable or a box.
 * Before you go and delete this, notice that we cannot use a vector of 
 * derived classes (like... block) as a vector of spatiable, they are 
 * just different classes... Yes, you can convert them to spatiable but,
 * what if you really wanted blocks instead of spatiables? A version with
 * a vector of pointers is also provided.
 */
	template<typename O>
	std::vector<O const *>          get_collisions(
		const std::vector<O>& _nodes
	) const {

		std::vector<O const*> result{};

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(subject, node)) {

				result.push_back(&node);
			}
		}

		return result;
	}

	template<typename O>
	std::vector<O const *>          get_collisions(
		const std::vector<O*>& _nodes
	) const {

		std::vector<O const*> result{};

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(subject, *node)) {

				result.push_back(node);
			}
		}

		return result;
	}

	private:

	d2d::collision::box       subject;
};

}}

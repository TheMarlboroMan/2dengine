#pragma once
#include "spatiable.h"
#include "tools.h"

#include <vector>

namespace d2d {namespace collision {

class checker {

	public:

	enum flags{
		flag_none=0,
		flag_skip_passable_side_check=1 //the box must be considered non-passable on all sides.
	};

	enum class phases{
		horizontal,
		vertical
	};

/**
 * generic get_collisions version that will return a vector of pointers to 
 * targets preserving their type. Does not take into account collision phase
 * at all, just checks if the nodes intersect with the target.
 */
	template<typename T>
	std::vector<T const *>          get_collisions(const d2d::collision::spatiable& _target, const std::vector<T>& _nodes) const {

		std::vector<T const*> result{};

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, node)) {

				result.push_back(&node);
			}
		}

		return result;
	}

/**
 * generic collision method that will return true if a collision is detected
 * and discard any other information.
 */
	template<typename T>
	bool                            has_collision(const d2d::collision::spatiable& _target, const std::vector<T>& _nodes) const {

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, node)) {

				return true;
			}
		}

		return false;
	}

/**
*returns true if there is a collision, takes phases and flags into account.
*assumes the subject and obstacle are both spatiables so the previous
*position can be taken into account when it matters. For collisions against
*boxes we can just use the free floating function "collides_with".
*/
	bool                            check(const d2d::collision::spatiable&, const d2d::collision::spatiable&, phases, int=0) const;
};

}}

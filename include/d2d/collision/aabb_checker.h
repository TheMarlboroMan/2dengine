#pragma once
#include "spatiable.h"
#include "tools.h"

#include <vector>

namespace d2d {namespace collision {

/**
*Collision checker for phase-separated aabb collision modes.
*/
class aabb_checker {

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
 * at all, just checks if the nodes intersect with the target. The target
 * type might be an spatiable or a box.
 * Before you go and delete this, notice that we cannot use a vector of 
 * derived classes (like... block) as a vector of spatiable, they are 
 * just different classes... Yes, you can convert them to spatiable but,
 * what if you really wanted blocks instead of spatiables?
 */
	template<typename T, typename O>
	std::vector<O const *>          get_collisions(
		const T& _target, 
		const std::vector<O>& _nodes
	) const {

		std::vector<O const*> result{};

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, node)) {

				result.push_back(&node);
			}
		}

		return result;
	}

/**
*Same as above but takes a vector of pointers.
*/
	template<typename T, typename O>
	std::vector<O const *>          get_collisions(
		const T& _target, 
		const std::vector<O*>& _nodes
	) const {

		std::vector<O const*> result{};

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, *node)) {

				result.push_back(node);
			}
		}

		return result;
	}

/**
 * generic collision method that will return true if a collision is detected
 * and discard any other information.
 */
	template<typename T, typename O>
	bool                            has_collision(
		const T& _target, 
		const std::vector<O>& _nodes
	) const {

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, node)) {

				return true;
			}
		}

		return false;
	}

/**
 * same as above but takes a vector of pointers.
 */
	template<typename T, typename O>
	bool                            has_collision(
		const T& _target, 
		const std::vector<const O*>& _nodes
	) const {

		for(const auto& node : _nodes) {

			if(d2d::collision::collides_with(_target, *node)) {

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
	bool                            check(
		const d2d::collision::spatiable&, 
		const d2d::collision::spatiable&, 
		phases,
		int=0
	) const;
};

}}

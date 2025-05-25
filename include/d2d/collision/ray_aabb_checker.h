#pragma once

#include "definitions.h"
#include "spatiable.h"


namespace d2d { namespace collision {

/*
TODO: I don't like how these templates are really "bad" for type checking...
what I actually want to say is that error messages can be really messy 
here. I guess I could learn "concepts" and stuff for this.
*/

/**
 * checks for moving spatiables against another spatiables. Gives its responses
 * in terms of booleans or lists of spatiables that would be in collision so
 * it is not efficient to calculate collision responses, as these must run the
 * same checks again.
 */
class ray_aabb_checker {

	public:

/**
 * checks if the last spatiable is in collision with the second, as the second
 * moves through the ray "ray". Assumes the ray emanates from the center of 
 * the second spatiable.
 */
	bool                        check(
		const ray&, 
		const d2d::collision::spatiable&, 
		const d2d::collision::spatiable&
	) const;

/**
 * Given a box/spatiable T and a vector of O (which can behave as spatiables)
 * returns a vector of pointers to O that collide with T as T moves towards
 * its ray.
 */
	template<typename T, typename O>
	std::vector<O const *> get_collisions(
		const ray& _ray, 
		const T& _target,
		const std::vector<O>& _nodes
	) const {

		std::vector<O const*> result{};
		for(const auto& node : _nodes) {

			if(this->check(_target, node)) {

				result.push_back(&node);
			}
		}

		return result;
	}

/**
 * same as above, but takes a vector of pointers to O
 */

	template<typename T, typename O>
	std::vector<O const *> get_collisions(
		const ray& _ray, 
		const T& _target,
		const std::vector<const O *>& _nodes
	) const {

		std::vector<O const*> result{};
		for(const auto& node : _nodes) {

			if(this->check(_target, node)) {

				result.push_back(node);
			}
		}

		return result;
	}

/**
 * Returns true if the spatiable/box T, moving through its ray collides with
 * any of the O (behaving as spatiables).
 */
	template<typename T, typename O>
	bool                            has_collision(
		const ray& _ray, 
		const T& _target, 
		const std::vector<O>& _nodes
	) const {

		for(const auto& node : _nodes) {

			if(this->check(_ray, _target, node)) {

				return true;
			}
		}

		return false;
	}

/**
 * same as above, but with pointers to O
 */
	template<typename T, typename O>
	bool                            has_collision(
		const ray& _ray, 
		const T& _target, 
		const std::vector<const O *>& _nodes
	) const {

		for(const auto& node : _nodes) {

			if(this->check(_ray, _target, *node)) {

				return true;
			}
		}

		return false;
	}
};

}}

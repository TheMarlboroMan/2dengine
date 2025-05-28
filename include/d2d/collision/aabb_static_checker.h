#pragma once

#include "spatiable.h"
#include "../tools/to_ref.h"

namespace d2d { namespace collision {

/**
 * Collision checker for static moments, does not attempt to know where the
 * collision came from or how to solve it, just if a collision takes place
 * between a box/spatiable and other spatiables.
 */
class aabb_static_checker {

	public:

	                        aabb_static_checker(const d2d::collision::spatiable&);
	                        aabb_static_checker(const d2d::collision::box&);
/**
 * clears the results.
 */
	aabb_static_checker&    reset();


	bool                    has_collision() const {return collision_found;}

/**
 * sets the early exit for the next call to detect_*
 */
	aabb_static_checker&      early_exit(bool);

/**
 * resets all modifiers (early exit). All modifiers are reset after
 * a call to detect_*
 */
	aabb_static_checker&      reset_modifiers();

/**
 * Checks and adds the result of checking the collision against the given 
 * spatiable. Returns the first collision that can be found (not neccesarily
 * the closest one!).
 */
	aabb_static_checker&      detect_one(const d2d::collision::spatiable&);
	aabb_static_checker&      detect_one(const d2d::collision::spatiable * _node) {return detect_one(*_node);}

/**
 * runs through all given nodes checking collisions and adding their results.
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 */
	template<typename T>
	aabb_static_checker&      detect_all(
		T& _nodes
	) {

		for(const auto& node : _nodes) {

			detect_one(d2d::tools::to_ref(node));
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

/**
 * same as the one before, but with a callable that can turn an element of
 * the container into an spatiable. If used with a callable we must make 
 * sure that it does not return a temporary! Lambdas with no return type
 * specified are tricky for this.
 */
	template<typename T, typename D>
	aabb_static_checker&      detect_all(
		T& _nodes,
		const D& _dereferencer
	) {

		for(const auto& node : _nodes) {

			const auto& spatiable=_dereferencer(node);
			detect_one(d2d::tools::to_ref(spatiable));
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

/**
 * runs through all given nodes checking collisions and adding their results
 * as long as the _skipper functor returns true. 
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 */
	template<typename T, typename P>
	aabb_static_checker&     detect_if(
		T& _nodes,
		const P& _skipper
	) {

		for(const auto& node : _nodes) {

			const auto& ref=d2d::tools::to_ref(node);
			if(!_skipper(ref)) {

				continue;
			}

			detect_one(ref);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		reset_modifiers();
		return *this;
	}

	const std::vector<spatiable const*>& get_results() const {return results;}

	private:

	bool                            collision_found{false},
	                                with_early_exit{false};
	const d2d::collision::box       subject;
	std::vector<spatiable const*>   results;
};

}}

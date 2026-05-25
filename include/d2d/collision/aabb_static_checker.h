#pragma once

#include "spatiable.h"
#include "../tools/to_ref.h"

//TODO
#include <iostream>

namespace d2d { namespace collision {

/**
 * Collision checker for static moments, does not attempt to know where the
 * collision came from or how to solve it, just if a collision takes place
 * between a box/spatiable and other spatiables. 
 * Does not preserve the types when retrieving results, treating everything
 * as an spatiable.
 * The way early exit works can cancel any call to detect_* if a collision
 * was previously detected unless the state is reset.
 */
class aabb_static_checker {

	public:

	//!Second parameter indicates with early exit.
	                        aabb_static_checker(const d2d::collision::spatiable&, bool=false);
	//!Second parameter indicates with early exit.
	                        aabb_static_checker(const d2d::collision::box&, bool=false);
/**
 * clears the results
 */
	aabb_static_checker&    reset();


	bool                    has_collision() const {return collision_found;}

/**
  sets the early exit for all calls to detect_all and detect_if. The way
 * this works is that 
 */
	aabb_static_checker&      set_early_exit(bool);

/**
 * resets the subject
 */

	aabb_static_checker&      set_subject(const d2d::collision::spatiable&);
	aabb_static_checker&      set_subject(const d2d::collision::box&);

/**
 * Checks and adds the result of checking the collision against the given 
 * spatiable. 
 */
	aabb_static_checker&      detect_one(const d2d::collision::spatiable&);
	aabb_static_checker&      detect_one(const d2d::collision::spatiable * _node) {return detect_one(*_node);}

/**
 * runs through all given nodes checking collisions and adding their results.
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 * Assumes nodes are spatiables or pointers to them. If the pointers are not
 * spatiables but can behave as one, check the overload with the dereferencer.
 */
	template<typename T>
	aabb_static_checker&      detect_all(
		const T& _nodes
	) {

		if(collision_found && with_early_exit) {

			return *this;
		}

		for(const auto& node : _nodes) {

			detect_one(d2d::tools::to_ref(node));
			if(collision_found && with_early_exit) {

				break;
			}
		}

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
		const T& _nodes,
		const D& _dereferencer
	) {

		if(collision_found && with_early_exit) {

			return *this;
		}

		for(const auto& node : _nodes) {

			const auto& spatiable=_dereferencer(node);
			detect_one(d2d::tools::to_ref(spatiable));
			if(collision_found && with_early_exit) {

				break;
			}
		}

		return *this;
	}

/**
 * runs through all given nodes checking collisions and adding their results
 * as long as the _skipper functor returns true. 
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 * Assumes nodes are spatiables. If they are not but can behave as one,
 * check the overload with the dereferencer.
 */
	template<typename T, typename P>
	aabb_static_checker&     detect_if(
		const T& _nodes,
		const P& _skipper
	) {

std::cout<<"detect_if will run through "<<_nodes.size()<<" nodes\n";

		if(collision_found && with_early_exit) {

			return *this;
		}



int i=0;
std::cout<<"detect_if will start processing now\n";
		for(const auto& node : _nodes) {

std::cout<<"node "<<i<<"\n";
i++;

			const auto& ref=d2d::tools::to_ref(node);
			if(!_skipper(ref)) {

				continue;
			}

			detect_one(ref);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		return *this;
	}

/**
 * Same as above... with a dereferencer. If used with a callable we must make 
 * sure that it does not return a temporary! Lambdas with no return type
 * specified are tricky for this. The skipper works against the original type
 * and not against the dereferenced one!
 */
	template<typename T, typename P, typename D>
	aabb_static_checker&     detect_if(
		const T& _nodes,
		const P& _skipper,
		const D& _dereferencer
	) {

std::cout<<"detect_if will run through "<<_nodes.size()<<" nodes\n";
		if(collision_found && with_early_exit) {

			return *this;
		}

int i=0;
std::cout<<"detect_if will start processing now\n";
		for(const auto& node : _nodes) {

std::cout<<"node "<<i<<"\n";
i++;
			if(!_skipper(node)) {

				continue;
			}

			const auto& ref=_dereferencer(node);
			detect_one(ref);
			if(collision_found && with_early_exit) {

				break;
			}
		}

		return *this;
	}

	const std::vector<spatiable const*>& get_results() const {return results;}

	private:

	bool                            collision_found{false},
	                                with_early_exit{false};
	d2d::collision::box             subject;
	std::vector<spatiable const*>   results;
};

}}

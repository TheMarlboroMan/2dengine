#pragma once

#include <d2d/collision/spatiable.h>
#include <d2d/collision/aabb_checker.h>
#include <d2d/collision/aabb_solver.h>
#include <d2d/tools/to_ref.h>

namespace d2d { namespace collision {

/**
 * a collision phase starts when constructed. After that, detect_one or 
 * detect_all can be called many times to check for collisions. At any moment
 * "has_collision" can be called to check if a collision took place and then
 * "response_generic" and "response_complex" can be used to examine the 
 * result.
 * Phases can be "horizontal" or "vertical" with slightly different solving
 * rules.
 */
class aabb_phase{

	public:

	                    aabb_phase(d2d::collision::spatiable&, d2d::collision::aabb_checker::phases);

	bool                has_collision() const {return collision_found;}

/**
 * restores original collision status to that the object can be 
 * reused
 */
	aabb_phase&              reset();

/**
 * resets all modifiers (flags / early exit). All modifiers are reset after
 * a call to detect_*
 */
	aabb_phase&              reset_modifiers();

/**
 * sets the given flags for the next call to detect_*
 */
	aabb_phase&              flags(int);

/**
 * sets the early exit for the next call to detect_*
 */
	aabb_phase&              early_exit(bool);

/**
 * Checks and adds the result of checking the collision against given spatiable.
 * Returns true if there was a collision. Beware, that does not mean that
 * this spatiable is the "closest" collision that will be picked up at
 * response time. Does not reset the given flags.
 */
	bool                detect_one(const d2d::collision::spatiable&, int=0);
	bool                detect_one(const d2d::collision::spatiable * _node, int _flags=0) {return detect_one(*_node, _flags);}

/**
 * runs through all given nodes checking collisions and adding their results.
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 */
	template<typename T>
	bool                detect_all(
		T& _nodes
	) {

		bool had_collision=false;
		for(const auto& node : _nodes) {

			if(detect_one(d2d::tools::to_ref(node), collision_flags)) {

				had_collision=true;
				if(with_early_exit) {

					break;
				}
			}
		}

		collision_found|=had_collision;
		reset_modifiers();
		return had_collision;
	}

/**
 * same as the one before, but with a callable that can turn an element of
 * the container into an spatiable. If used with a callable we must make 
 * sure that it does not return a temporary! Lambdas with no return type
 * specified are tricky for this.
 */
	template<typename T, typename D>
	bool                detect_all(
		T& _nodes,
		const D& _dereferencer
	) {

		bool had_collision=false;
		for(const auto& node : _nodes) {

			const auto& spatiable=_dereferencer(node);
			if(detect_one(d2d::tools::to_ref(spatiable), collision_flags)) {

				had_collision=true;
				if(with_early_exit) {

					break;
				}
			}
		}

		collision_found|=had_collision;
		reset_modifiers();
		return had_collision;
	}

/**
 * runs through all given nodes checking collisions and adding their results
 * as long as the _skipper functor returns true. 
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be. Works on containers of pointers or not.
 */
	template<typename T, typename P>
	bool                detect_if(
		T& _nodes,
		const P& _skipper
	) {

		bool had_collision=false;
		for(const auto& node : _nodes) {

			const auto& ref=d2d::tools::to_ref(node);
			if(!_skipper(ref)) {

				continue;
			}

			if(detect_one(ref, collision_flags)) {

				had_collision=true;
				if(with_early_exit) {

					break;
				}
			}
		}

		collision_found|=had_collision;
		reset_modifiers();
		return had_collision;
	}

/**
 * invokes a generic response that will adjust position. guaranteed to do
 * nothing if there was no collision. 
 */
	void                        response_generic();

/**
 * attempts to return the complex response. Will throw if
 * there was no collision!
 */
	d2d::collision::aabb_response   response_complex();

	private:

	bool                            collision_found{false},
	                                with_early_exit{false};
	int                             collision_flags{0};
	d2d::collision::aabb_checker::phases collision_phase;
	d2d::collision::spatiable&      subject;
	d2d::collision::aabb_checker    checker;
	std::vector<spatiable const*>   results;
};

}}

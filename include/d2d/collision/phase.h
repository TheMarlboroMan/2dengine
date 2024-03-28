#pragma once

#include <d2d/collision/spatiable.h>
#include <d2d/collision/checker.h>
#include <d2d/collision/solver.h>
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
class phase{

	public:

	                    phase(d2d::collision::spatiable&, d2d::collision::checker::phases);

	bool                has_collision() const {return collision_found;}

/**
 * Checks and adds the result of checking the collision against given spatiable.
 * Returns true if there was a collision. Beware, that does not mean that
 * this spatiable is the "closest" collision that will be picked up at
 * response time.
 */
	bool                detect_one(const d2d::collision::spatiable&, int=0);
	bool                detect_one(const d2d::collision::spatiable * _node, int _flags=0) {return detect_one(*_node, _flags);}

/**
 * runs through all given nodes checking collisions and adding their results.
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be.
 */
	template<typename T>
	bool                detect_all(
		T& _nodes,
		int _flags=0,
		bool _exit_on_collision=false
	) {

		bool had_collision=false;
		std::size_t l=_nodes.size(); 
		for(std::size_t i=0; i<l; i++) {

			if(detect_one(d2d::tools::to_ref(_nodes[i]), _flags)) {

				had_collision=true;
				if(_exit_on_collision) {

					break;
				}
			}
		}

		collision_found|=had_collision;
		return had_collision;
	}

/**
 * runs through all given nodes checking collisions and adding their results
 * as long as the _skipper functor returns true. 
 * Returns true if there was any collision with any of the nodes. Can opt
 * for an early exit if need be.
 */
	template<typename T, typename P>
	bool                detect_if(
		T& _nodes,
		const P& _skipper, 
		int _flags=0,
		bool _exit_on_collision=false
	) {

		bool had_collision=false;
		std::size_t l=_nodes.size(); 
		for(std::size_t i=0; i<l; i++) {

			if(!_skipper(d2d::tools::to_ref(_nodes[i]))) {

				continue;
			}

			if(detect_one(d2d::tools::to_ref(_nodes[i]), _flags)) {

				had_collision=true;
				if(_exit_on_collision) {

					break;
				}
			}
		}

		collision_found|=had_collision;
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
	d2d::collision::response    response_complex();

	private:

	bool                            collision_found{false};
	d2d::collision::checker::phases collision_phase;
	d2d::collision::spatiable&      subject;
	d2d::collision::checker         checker;
	std::vector<spatiable const*>   results;
};

}}

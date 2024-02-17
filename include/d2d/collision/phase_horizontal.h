#pragma once

#include <d2d/collision/spatiable.h>
#include <d2d/collision/checker.h>

namespace d2d { namespace collision {

class phase_horizontal {

	public:

	                    phase_horizontal(d2d::collision::spatiable&);

	bool                has_collision() const {return collision_found;}

	bool                detect_one(const d2d::collision::spatiable&, int=0);

	template<typename T>
	bool                detect_all(const T& _nodes, int _flags=0, bool _exit_on_collision=false) {

		bool had_collision=false;
		for(const auto& node : _nodes) {

			if(detect_one(node, _flags)) {

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
 * nothing if there was no collision. Can be only invoked once per collision
 * cycle!
 * TODO: Actually, this is not guaranteed to be checked that can be invoked
 * only once and I am not even sure that is a great idea :S.
 */
	void                        response_generic();

	private:

	bool                        collision_found{false};
	d2d::collision::spatiable&  subject;
	d2d::collision::checker 	checker;
};

}}

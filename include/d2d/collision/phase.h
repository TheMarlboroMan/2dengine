#pragma once

#include <d2d/collision/spatiable.h>
#include <d2d/collision/checker.h>
#include <d2d/tools/to_ref.h>

namespace d2d { namespace collision {

class phase{

	public:

	                    phase(d2d::collision::spatiable&, d2d::collision::checker::phases);

	bool                has_collision() const {return collision_found;}

	bool                detect_one(const d2d::collision::spatiable&, int=0);
	bool                detect_one(const d2d::collision::spatiable * _node, int _flags=0) {return detect_one(*_node, _flags);}

	template<typename T>
	bool                detect_all(T& _nodes, int _flags=0, bool _exit_on_collision=false) {

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
 * invokes a generic response that will adjust position. guaranteed to do
 * nothing if there was no collision. 
 */
	void                        response_generic();

	private:

/**
 * to reference helpers to detect all can be called with any kind of vectors,
 * pointers or not.
 */

	bool                            collision_found{false};
	d2d::collision::checker::phases collision_phase;
	d2d::collision::spatiable&      subject;
	d2d::collision::checker         checker;
	std::vector<spatiable const*>   results;
};

}}

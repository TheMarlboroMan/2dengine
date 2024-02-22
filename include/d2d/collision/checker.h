#pragma once
#include "spatiable.h"

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
	* Returns a vector of the collisions between the box and the boxes in 
	* a single pass. Needs a vector of spatiables, which must likely be
	* casted beforehand.
	*/
	std::vector<spatiable const *>  get_collisions(const d2d::collision::box&, const std::vector<spatiable>&);
	std::vector<spatiable const *>  get_collisions(const d2d::collision::box&, const std::vector<spatiable const *>&);

//TODO: can we have some templates here so we can check any kind of stuff?
//TODO: maybe check enable_if??

/**
*returns true if there is a collision, takes phases and flags into account.
*assumes the subject and obstacle are both spatiables so the previous
*position can be taken into account when it matters. For collisions against
*boxes we can just use the free floating function "collides_with".
*/
	bool                            check(const d2d::collision::spatiable&, const d2d::collision::spatiable&, phases, int=0) const;

//TODO: This can be quarantined and moved to some other class if need be.
	/**
	*starts a collision sequence agaist the given box. Collision candidates
	*must be passed individually through "add" and "end" must be called 
	*when done. Each call to start clears the result buffer.
	*The versions with two arguments use the second box as the previous 
	*position of the given box (useful for things that may be passable from
	*below only, for example).
	*/

//	void                            start(const d2d::collision::box&, phases);
//	void                            start(const d2d::collision::spatiable&, phases);
	/**
	*adds a node to the sequence. returns true if there is a collision in 
	*with the node allowing for - among others - early loop exits. The node is 
	*added to a collision buffer if a collision exists.
	*/
//	bool                            add(const d2d::collision::spatiable&, int=0);
	/**
    *adds SOMETHING to the sequence. returns true in case of collision, can
	*perform early exit. Adds any collisions to the collision buffer.
	*/
/*
	template<typename T> 
	bool                            add_all(const T& _nodes, int _flags=0, bool _exit_on_collision=false) {

		bool had_collision=false;
		for(const auto& node : _nodes) {

			if(add(node, _flags)) {

				had_collision=true;
				if(_exit_on_collision) {

					break;
				}
			}
		}

		return had_collision;
	}
*/
	/**
    *finishes the sequence and returns the collision buffer. Can only 
	*be called once for each "start" call. 
	*/
//	std::vector<spatiable const *>  end();

	private:

//	d2d::collision::box             subject{};
//	d2d::collision::box             previous_position{};
//	bool                            started{false}; //for the sequential methods...
//	std::vector<spatiable const*>   results;
//	phases                          phase{phases::horizontal};
};

}}

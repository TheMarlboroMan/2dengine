#pragma once

#include "definitions.h"
#include "spatiable.h"

#include <vector>

namespace d2d {namespace collision {

class checker {

	public:

	/**
	* Returns a vector of the collisions between the box and the boxes in 
	* a single pass. Needs a vector of spatiables, which must likely be
	* casted beforehand.
	*/
	std::vector<spatiable const *>            get_collisions(const d2d::collision::box&, const std::vector<spatiable>&);
	std::vector<spatiable const *>            get_collisions(const d2d::collision::box&, const std::vector<spatiable const *>&);

	/**
	*starts a collision sequence agaist the given box. Collision candidates
	*must be passed individually through "add" and "end" must be called 
	*when done. Each call to start clears the result buffer.
	*/
	void                            start(const d2d::collision::box&);
	void                            start(const d2d::collision::spatiable&);
	/**
	*adds a node to the sequence. returns true if there is a collision in 
	*with the node allowing for - among others - early loop exits. The node is 
	*added to a collision buffer if a collision exists.
	*/
	bool                            add(const d2d::collision::spatiable&);
	/**
    *finishes the sequence and returns the collision buffer. Can only 
	*be called once for each "start" call. 
	*/
	std::vector<spatiable const *>  end();

	private:

	d2d::collision::box             subject{};
	bool                            started{false}; //for the sequential methods...
	std::vector<spatiable const*>   results;
};

}}

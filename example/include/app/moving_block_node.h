#pragma once

#include <d2d/collision/definitions.h>
#include <d2d/components/timeouts.h>
#include <ostream>

namespace app {

/**
 * A waypoint for moving blocks. If only that word, waypoint, would have come
 * to me sooner... Anyway, a waypoint acts as a stop for moving blocks, where
 * the moving block "steals" the next id, the velocity it will travel to
 * and how much to wait at the current station.
 */
class moving_block_node {

	public:

	                        moving_block_node(d2d::collision::point, int, int, int, int);

	//I'll just take the easy way and make these public.
	d2d::collision::point   point;
	int                     id,
	                        nextid,
	                        velocity, //velocity a block will travel to "nextid".
	                        wait_ms; //ms to wait at this node. 
};

std::ostream& operator<<(std::ostream&, const moving_block_node&);
}

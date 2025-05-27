#pragma once

#include "spatiable.h"
#include <vector>

#ifdef IS_DEBUG_BUILD
	#include <iostream>
#endif

namespace d2d { namespace collision {

/**
*this class can be fed a list of spatiables to watch and another to target.
*when tic() is called, watched that pushed targets will get their collisions
*resolved. It also allows for spatiables to be latched to watched nodes,
*so that they move along with them (think "riding a platform").
*A possible workflow is to reset, then watch all moving stuff and target all 
*pushable nodes at the "map loading" point, then attach if needed, then tic 
*after the "world_tic" is done. 
*Of course, any illegal positions and shuch must be taken care of by the 
*application itself.
**/

struct collision_tracker_correction {

	const spatiable *           watched;
	spatiable *                 target;
	d2d::motion::motion_vector  vector;
	box_edge                    edge; //snap to this edge of
	bool                        is_snap() const {return vector.x==0. && vector.y==0;}
}; 


//TODO: Sort of a stupid name.
class collision_tracker {

	public:

	std::size_t                     watched_size() const {return watched.size();}
	std::size_t                     target_size() const {return targets.size();}

/**
 * clears all state of the instance (watched and targets)
 */
	collision_tracker&              reset();

/**
 * performs all needed calculations to push targets around. Does not perform
 * movement of the watched spatiable list, that falls under the application's
 * responsibility. Returns a list of corrections to apply.
 */
	collision_tracker&              tic();

/**
 * TODO:
 */
	void                            push_correct(const collision_tracker_correction&); 

/**
 * Returns the results of the previous tic.
 */
	const std::vector<collision_tracker_correction>& get_corrections() const {return corrections;}

/**
 * adds a node to the "watched list" of spatiables that can push other
 * spatiables.Does not allow for repeated values. Does not allow for the
 * watched node to be also in the target list.
 */
	collision_tracker&              watch(const spatiable&);
/**
 * removes a node from the watched list, if found.
 */
	collision_tracker&              unwatch(const spatiable&);
/**
 * attaches the first spatiable to the second, which must be on the list. This
 * will cause the attached node to replicate the movement of the watched one
 * (including any weird jumps that we might want do make them do). Does not
 * allow for the target to be in the watched list. If the attached node is
 * also a target and gets pushed by the node it's attached to only the 
 * attachment movement will be produced.
 */
	collision_tracker&              attach(const spatiable&, spatiable&);
/**
 * attaches the first spatiable from the second, which must be on the 
 * watched list.
 */
	collision_tracker&              detach(const spatiable&, const spatiable&);
/**
 * detaches the spatiable from any element on the watched list.
 */
	collision_tracker&              detach_from_all(const spatiable&);
/**
 *Adds the element to the list of targets that can be pushed around by 
 *elements on the watch list. Does not allow for repeated targets or for
* elements that are already on the watch list.
 */
	collision_tracker&              target(spatiable&);
/**
 * removes the element from the target list, if found.
 */
	collision_tracker&              untarget(const spatiable&);

	private:

/**
 *Represents the body and each of the different spatiables that can be attached
 *to it (say, riding it, latched to its side, etc).
 */
	struct node {

		const spatiable*            body;
		std::vector<spatiable *>    attached;
	};

	std::vector<node>               watched; //all moving nodes.
	std::vector<spatiable*>         targets; //all targets that can be pushed around by nodes.
	std::vector<collision_tracker_correction> corrections;
};

}}

#pragma once

#include "spatiable.h"
#include "../motion/definitions.h"
#include <vector>
#include <functional>
#include <memory>

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
*Of course, any illegal positions and such must be taken care of by the 
*application itself.
**/

/**
 * Represents a "watched" pushing a "target".
 */
struct collision_tracker_correction {

	const spatiable *           watched;
	spatiable *                 target;
	box_edge                    edge; //snap to this edge of
}; 

/**
 * Represents the movement of a "target" riding a "watched".
 */
struct collision_tracker_passive_movement {

	const spatiable *           watched;
	spatiable *                 target;
	d2d::motion::motion_vector  vector;
};

//TODO: Sort of a stupid name.
class collision_tracker {

	public:

	/**
    * Policy object operator. These can be added to any watched node to specify 
    * if they will be able to push targets by answering the question 
    * using the first and second args (the node body and the target). The idea
    * here is to specialize this to include application specifics.
    */
	struct can_push_policy_interface {

		virtual bool operator()(const spatiable&, const spatiable&) const=0;
	};

	std::size_t                     watched_size() const {return watched.size();}
	std::size_t                     target_size() const {return targets.size();}

/**
 * returns true if the node is attached to any of the watched blocks.
 */
	bool                            is_attached(const spatiable&) const;

/**
 * returns the watched that the argument is attached to... Or null if none.
 */
	const spatiable *               get_host(const spatiable&) const;

/**
 * clears all state of the instance (watched and targets)
 */
	collision_tracker&              restart();

/**
 * Keeps watched and targets, but clears results and attachments!
 */
	collision_tracker&              reset();

/**
 * performs all needed calculations to push targets around. Does not perform
 * movement of the watched spatiable list, that falls under the application's
 * responsibility. Returns a list of corrections to apply.
 */
	collision_tracker&              tic();

/**
 * Corrects all snaps from the last tic. A snap is when a watcher pushes
 * a target around.
 */
	collision_tracker&              correct_snaps();

/**
 * Corrects all snaps from the last tic pertaining to the given spatiable,
 * that it, those that have the spatiable as the target in the correction
 * list. Returns an integer that is a flag of edge as defined in "definitions.h".
 * meaning that the target collided with the N sides of a watched node.
 */
	int                            correct_snaps(const spatiable&);

/**
 * Manually corrects a snap. Returns the edges as "the target collided with
 * that side of a watched node".
 */
	int                            correct_snap(const collision_tracker_correction&); 

/**
 * After a tic, attempts to grab a correction vector for the given spatiable
 * if it was attached to something.
 */
	d2d::motion::motion_vector      attached_vector_for(const spatiable&) const;

/**
 * Returns the snaps of the previous tic
 */
	const std::vector<collision_tracker_correction>& get_corrections() const {return corrections;}

/**
 * adds a node to the "watched list" of spatiables that can push other
 * spatiables.Does not allow for repeated values. Does not allow for the
 * watched node to be also in the target list. A policy can be added to 
 * add customization points, in this case a class that tells us if the node
 * can push a target around.
 */
	collision_tracker&              watch(const spatiable&, can_push_policy_interface* =nullptr);
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
		d2d::motion::motion_vector  previous_vector;
		std::unique_ptr<can_push_policy_interface> can_push_policy;
	};

	std::vector<node>               watched; //all moving nodes.
	std::vector<spatiable*>         targets; //all targets that can be pushed around by nodes.
	std::vector<collision_tracker_correction> corrections;
	std::vector<collision_tracker_passive_movement> passive_movements;
};

}}

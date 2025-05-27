#pragma once

#include "definitions.h"
#include "spatiable.h"

#include <vector>
#include <stdexcept>

namespace d2d { namespace collision {

struct aabb_response_exception
	:public std::runtime_error {
			aabb_response_exception(const std::string& _err)
				:std::runtime_error(
					std::string{"no collision was detected: "}+_err
				) {}
};

struct aabb_response {

	//read this as the subject collided with the N side of the obstacle.
	int                     edges{0};
	double                  magnitude{0.0};
	const collision::spatiable * obstacle{nullptr};
	bool                    is_crushing() const {

		return ((edges & top) && (edges & bottom))
			|| ((edges & left) && (edges & right));
	}

	void                    solve(spatiable&);
};

/**
* Collision solver for AABB with separated X and Y axis.
* Handles vertical and horizontal solid collisions by snapping the box with
* whatever it collided with... When it comes to colliding entities in 
* movement the order on which they are moving is critical here!
* Works in tandem with the aabb_checker that returns vectors of pointers to
* spatiables, that must be fed to this class. Assumes, thus, that there
* is actually a collision taking place there.
*/

class aabb_solver {

	public:

	//Solve against the first obstacle found.
	void                    horizontal(collision::spatiable&, const std::vector<collision::spatiable const *>&);
	void                    vertical(collision::spatiable&, const std::vector<collision::spatiable const *>&);
	//Solve against the nearest obstacle found!
	aabb_response           horizontal_complex(const collision::spatiable&, const std::vector<collision::spatiable const *>&);
	aabb_response           vertical_complex(const collision::spatiable&, const std::vector<collision::spatiable const *>&);

	//Indicates if we must use the previous or current box for spatiables.
	bool                    use_subject_previous{true},
	                        use_obstacle_previous{true};

};

}}

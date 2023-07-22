#pragma once

#include "definitions.h"
#include "spatiable.h"

#include <vector>
#include <stdexcept>

namespace d2d { namespace collision {

/**
* Handles vertical and horizontal solid collisions by snapping the box with
* whatever it collided with.
*/

struct response_exception
	:public std::runtime_error {
			response_exception(const std::string& _err)
				:std::runtime_error(
					std::string{"no collision was detected: "}+_err
				) {}
};

struct response {

	//read this as the subject collided with the N side of the obstacle.
	enum tedges             {top=1, bottom=2, right=4, left=8};
	int                     edges{0};
	double                  magnitude{0.0};
	const collision::spatiable * obstacle{nullptr};
	bool                    is_crushing() const {

		return ((edges & top) && (edges & bottom))
			|| ((edges & left) && (edges & right));
	}

	void                    solve(spatiable&);
};

class solver {

	public:

	void                    horizontal(collision::spatiable&, const std::vector<collision::spatiable const *>&);
	void                    vertical(collision::spatiable&, const std::vector<collision::spatiable const *>&);
	response                horizontal_complex(const collision::spatiable&, const std::vector<collision::spatiable const *>&);
	response                vertical_complex(const collision::spatiable&, const std::vector<collision::spatiable const *>&);

};

}}

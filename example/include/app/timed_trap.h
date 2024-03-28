#pragma once

#include "entity.h"
#include <d2d/components/timeouts.h>
#include <iostream>

namespace app {

/**
 * a timed trap is something lethal to the touch that has a pre-count, a
 * count and a post-count, so a few of them can be sinchronized.
 */
class timed_trap {

	public:

	enum class types {
		fire
	};

/**
 * integer args are pre-count, duration and post-count
 */
	                    timed_trap(d2d::collision::point, types, int, int, int);

	void                tic(float);
	bool                is_harmful() const {return state==states::harm;}
	types               get_type() const {return type;}

	entity              ent;

	private:

	enum class states {
		pre,
		harm,
		post
	}                           state{states::pre};

	types                       type{types::fire};

	float                       pre_s,
	                            harm_s,
	                            post_s;
	d2d::components::timeout    timeout;

	static const int    fire_w{10};
	static const int    fire_h{12};
};

std::ostream& operator<<(std::ostream&, const timed_trap&);
}


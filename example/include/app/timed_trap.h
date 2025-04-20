#pragma once

#include "entity.h"
#include <d2d/components/timeouts.h>
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * a timed trap is something lethal to the touch that has a pre-count, a
 * count and a post-count, so a few of them can be sinchronized. The player
 * will be defeated if entering into its area when active.
 */
class timed_trap {

	public:

	enum class types {
		fire
	};

	//Public, so the thing loader can align it to a grid.
	static const int    fire_w{6};

/**
 * integer args are tag, pre-count, duration and post-count
 */
	                    timed_trap(d2d::collision::point, types, bool, int, int, int, int);

	int                 tic(ldtools::tdelta);
	bool                is_harmful() const {return active && state==states::harm;}
	types               get_type() const {return type;}
	bool                is_active() const {return active;}
	int                 get_tag() const {return tag;}
	void                activate() {active=true;}

	entity              ent;

	private:

	enum class states {
		pre,
		harm,
		post
	}                           state{states::pre};

	types                       type{types::fire};

	int                         tag{0};
	bool                        active{true};
	double                      pre_s,
	                            harm_s,
	                            post_s;
	d2d::components::timeout    timeout;

	static const int    fire_h{14};
};

std::ostream& operator<<(std::ostream&, const timed_trap&);
}


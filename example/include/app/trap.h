#pragma once

#include "entity.h"
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * a trap is something lethal to the touch that can be active or inactive.
 */
class trap {

	public:

	enum class types {
		fire,
		spikes
	};

	//Public, so the thing loader can align it to a grid.
	static const int    fire_w{6};
	static const int    spikes_w{16};

	                    trap(d2d::collision::point, types, bool, bool, int);

	types               get_type() const {return type;}
	bool                is_active() const {return active;}
	int                 get_tag() const {return tag;}
	void                toggle() {active=!active;}
	void                reset();

	entity              ent;

	private:

	types                       type{types::fire};

	int                         tag{0};
	bool                        active{true},
	                            starting_active{true},
	                            keep_active_when_reset{true};

	static const int    fire_h{14};
	static const int    spikes_h{8};
	friend std::ostream& operator<<(std::ostream&, const trap&);

};

std::ostream& operator<<(std::ostream&, const trap&);
}


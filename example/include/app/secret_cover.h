#pragma once

#include "entity.h"
#include <d2d/components/timeouts.h>
#include <iostream>
#include <ldtools/time_definitions.h>

namespace app {

/**
 * A fancy name for a black rectangle covering part of the map that must
 * be uncovered when the player walks over it.
 * Is identified by an unique id so that it can be persisted over repeated
 * visits.
 */
class secret_cover {

	public:

	                            secret_cover(d2d::collision::box, int);

	entity                      ent;
	int                         id;

	bool                        is_discovered() const;
	bool                        is_hidden() const;
	bool                        is_dissapearing() const;

	void                        tic(ldtools::tdelta);
	void                        discover();
	ldtools::tdelta             get_timer() const {return timeout.get();}

	private:

	enum class states {
		hidden,
		dissapearing,
		discovered
	}                           state{states::hidden};

	d2d::components::timeout    timeout;
};

std::ostream& operator<<(std::ostream&, const secret_cover&);

}

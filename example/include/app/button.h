#pragma once

#include "entity.h"
#include <iostream>
#include <d2d/components/timeout.h>

namespace app {

using ldtools::tdelta;

/**
 * cannot be named "switch".
 */
class button {

	public:

	enum class types {
		regular,
		yellow_keyhole,
		blue_keyhole,
		red_keyhole,
		green_keyhole
	};

	                                button(d2d::collision::point, types, int, int, bool, bool, bool);
	void                            reset();
	void                            toggle();
	void                            tic(tdelta);
	bool                            can_be_activated() const;

	entity                          ent;
	types                           type;
	int                             id;
	int                             tag;
	bool                            used,
	//Stay "used" when the level resets or is re-entered. For persistent buttons.
	                                keep_used_when_reset,
	//Can be used more than once. Stores "used" state in persistence groups.
	                                repeatable;

	private:

	d2d::components::timeout        timeout;
	static constexpr double         timeoutval{0.2};
};

std::ostream& operator<<(std::ostream&, const button&);
}

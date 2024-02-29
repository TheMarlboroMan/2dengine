#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>

namespace app {

class player {

	public:

	entity                                      ent;
	d2d::motion::motion_vector                  velocity{0.0, 0.0};
	faces                                       facing{faces::right};
	bool                                        jump_shortened{false};

	enum class states {
		ground,
		air,
		ladder
	} state{states::ground};

};
}

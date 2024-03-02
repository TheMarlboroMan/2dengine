#pragma once

#include "entity.h"
#include "definitions.h"
#include <d2d/components/timeouts.h>
#include <d2d/motion/definitions.h>

namespace app {

struct ladder;

class player {

	public:

	enum timeout_indexes {
		timeout_ladder=0,
		timeout_last_jump_chance=1,
		timeout_defeat=2
	};
	                                            player();
	entity                                      ent;
	d2d::motion::motion_vector                  velocity{0.0, 0.0};
	faces                                       facing{faces::right};
	bool                                        jump_shortened{false};
	d2d::components::timeouts                   timeouts;
	const app::ladder*                          current_ladder{nullptr};

	void                                        tic(float);

	enum class states {
		ground,
		air,
		ladder,
		crouch,
		defeat
	} state{states::ground};

};
}

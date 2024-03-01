#pragma once

namespace app {

struct player_input {

	int         x=0,
	            y=0;
	bool        jump{false},
	            hold_jump{false},
	            enter_door{false};

	operator bool() const {

		return x || y || jump || hold_jump || enter_door;
	}
};
}

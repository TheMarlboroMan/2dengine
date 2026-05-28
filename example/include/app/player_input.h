#pragma once

namespace app {

struct player_input {

	int         x=0,
	            y=0;
	bool        jump{false},
	            hold_jump{false},
	            activate{false};

	void        reset();
	operator    bool() const;
};
}

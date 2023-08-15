#pragma once

namespace app {

struct player_input {

	int         x=0,
				y=0;

	operator bool() const {

		return x || y;
	}
};
}

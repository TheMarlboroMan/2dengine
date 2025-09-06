#pragma once

namespace app {

//Notice the main game inputs are powers of 2 so they can be stored in 8 bits.
//Later inputs, such as "tic" and "reload_values" are for debug purposes only
//and should not be stored.
enum input {
	escape=1,
	left=2,
	right=4,
	up=8,
	jump=16,
	down=32,
	pause=64
	//begin non-game inputs.
#ifdef IS_DEBUG_BUILD
	,
	tic=128,
	reload_values=256
#endif
};
}

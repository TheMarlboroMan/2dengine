#pragma once

namespace controller {

enum states {
	state_min=0,
	state_menu,
	state_main,
	state_pause,
	state_controls,
	state_options,
#ifdef IS_DEBUG_BUILD
	state_test,
#endif
	state_max
};
}

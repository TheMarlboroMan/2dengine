#pragma once

#include "app/env.h"

#include <string>
#include <vector>

namespace app {

struct save_slot {

	bool                    new_game{false};
	std::string             filename{};
	int                     collectibles{0},
	                        skill_setting{0}, //expressed in flags!
	                        elapsed_seconds{0};
};

class savegame_manager {

	public:

	                        savegame_manager(const appenv::env&);
	void                    load();
	const std::vector<save_slot>& get_slots() const {return slots;}

	private:

	const appenv::env&      env;
	std::vector<save_slot>  slots;
};

}

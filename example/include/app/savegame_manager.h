#pragma once

#include "app/env.h"

#include <string>
#include <vector>

namespace app {

struct save_slot {

	bool                    new_game{false};
	std::string             filename{},
							levelname{};
	int                     collectibles{0},
	                        skill_setting{0}, //expressed in flags!
	                        elapsed_seconds{0};
};

class savegame_manager {

	public:

	                        savegame_manager(const appenv::env&);
/**
 * loads game slots with info.
 */
	void                    load();
/**
 * erases one of the given slots, reloads. The slot MUST not be an empty
 * game or we will throw. 
 */
	void                    erase(std::size_t);
/**
 * returns all game slots
 */
	const std::vector<save_slot>& get_slots() const {return slots;}
/**
 * returns a single game slot, throws when out of bounds.
 */
	const save_slot&        get(std::size_t) const;
/**
 * returns the amount of slots. Not all slots will contain a saved game but
 * all will contain a save_slot object.
 */
	std::size_t             size() const {return slots.size();}

	private:

	const appenv::env&      env;
	std::vector<save_slot>  slots;
};

}

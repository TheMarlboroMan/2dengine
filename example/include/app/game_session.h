#pragma once

#include <string>
#include <tools/chrono.h>

namespace app {

struct game_session {

	int     skill_level{0},
	        lives{0},
	        elapsed_seconds{0}; 
	std::string savegame_file;

/**
 * this is the current session timer. For a single play sesion it is enough
 * to use it to know how long we have been playing. However, for a loaded game
 * we also need to add the "seconds_elapsed" property.
 */
	tools::chrono game_clock;
/**
 * an easy way to know where we are for the automap. This is just a number,
 * which is different from the map filename that is used in the game 
 * controller to know what we are supposed to load. The automap identifies maps
 * by their unique numeric id.
 */
	int     current_map_id{0};

	bool    with_lives() const;
	bool    with_timer() const;
	void    reset(int, const std::string&);
	void    set_skill_level(int);
};
}

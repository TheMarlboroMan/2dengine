#pragma once

namespace app {

struct game_session {

	int     skill_level{0},
	        lives{0},
	        seconds_elapsed{0},
/**
 * an easy way to know where we are for the automap. This is just a number,
 * which is different from the map filename that is used in the game 
 * controller to know what we are supposed to load. The automap identifies maps
 * by their unique numeric id.
 */
	        current_map_id{0}; 

	bool    with_lives() const;
	bool    with_timer() const;
	void    reset(int);
	void    set_skill_level(int);
};
}

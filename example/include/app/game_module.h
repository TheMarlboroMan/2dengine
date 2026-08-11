#pragma once

#include <string>

namespace app {

/**
 *puny name for the class that we use to group everything pertaining to the
* game's levelset: where are the maps? what's the starting map? how much 
* stuff is 100%, etc etc. The idea is to make the game modular, hence "module".
* No, I'm not being clever here.
 */
class game_module {

	public:

	unsigned int        get_total_rooms() const {return 200;}
	unsigned int        get_total_treasure() const {return 100;}
	//! String that will be appended to savegames, like savegame_###_001.
	std::string  get_savefile_affix() const {return "tdt";}
	//! Directory where maps are.
	//TODO: env.build_app_path("resources/maps/")
	std::string  get_map_path() const {return "resources/thedreamingtower/maps/";}
	///! Path of the automap file.
	//TODO: //get_env().build_app_path("resources/lists/automap.txt")
	std::string  get_automap_file() const {return "resources/thedreamingtower/automap.txt";}
	//! Name of the starting map, no extension, always assume json!
	//TODO:
	std::string  get_start_mapname() const {return "intro_001";}
	//TODO:
	//! Name of the ending map, no extension, always assume json!
	std::string  get_end_mapname() const {return "ending_001";}
	//! Directory where language files are.
	std::string  get_localization_path() const {return "resources/thedreamingtower/localization";}
};
}

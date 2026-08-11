#pragma once

#include <string>
#include <lm/log.h>

namespace app {

/**
 *puny name for the class that we use to group everything pertaining to the
* game's levelset: where are the maps? what's the starting map? how much 
* stuff is 100%, etc etc. The idea is to make the game modular, hence "module".
* No, I'm not being clever here.
 */
class game_module {

	public:

	                    game_module(const std::string&, lm::logger&);

	unsigned int        get_total_rooms() const {return total_rooms;}
	unsigned int        get_total_treasure() const {return total_treasure;}
	//! String that will be appended to savegames, like savegame_###_001.
	const std::string&  get_savefile_affix() const {return savegame_name;}
	//! Directory where maps are.
	const std::string&  get_map_path() const {return maps_path;}
	///! Path of the automap file.
	const std::string&  get_automap_file() const {return automap_path;}
	//! Name of the starting map, no extension, always assume json!
	const std::string&  get_start_mapname() const {return startmap;}
	//! Name of the ending map, no extension, always assume json!
	const std::string&  get_end_mapname() const {return endmap;}
	//! Directory where language files are.
	std::string  get_localization_path() const {return localization_path;}

	private:

	lm::logger&         logger;

	void                load(const std::string&);
	void                parse_module_file(const std::string&);

	std::string         startmap,
	                    endmap,
	                    savegame_name,
	                    maps_path,
	                    automap_path,
	                    localization_path;
	int                 total_rooms,
	                    total_treasure;
};
}

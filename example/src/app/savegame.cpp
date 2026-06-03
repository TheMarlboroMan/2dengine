#include "app/savegame.h"
#include <tools/string_utils.h>

#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace app;

savegame::savegame(
	const std::string& _map_filename,
	const std::string& _persistence_string,
	int _entry_id,
	int _difficulty_setting,
	int _elapsed_seconds,
	int _discovered_rooms,
	int _lives,
	int _yellow_keys,
	int _blue_keys,
	int _red_keys,
	int _green_keys,
	int _white_keys,
	int _treasure,
	int _ultimate
):
	map_filename(_map_filename),
	persistence_string{_persistence_string},
	entry_id{_entry_id},
	difficulty_setting{_difficulty_setting},
	elapsed_seconds{_elapsed_seconds},
	discovered_rooms{_discovered_rooms},
	lives{_lives},
	yellow_keys{_yellow_keys},
	blue_keys{_blue_keys},
	red_keys{_red_keys},
	green_keys{_green_keys},
	white_keys{_white_keys},
	treasure{_treasure},
	ultimate{_ultimate}
{}

savegame savegame::from_default() {

	return savegame(
		"", "", //mapname, persistence
		-1, -1, //entry, skill,
		-1, -1, -1,  //seconds, rooms, lives
		-1, -1, -1, -1, -1,//keys y, b, r, g, w.
		-1, -1 //treasure and ultimate.
	); 
}


savegame savegame_io::load_from_file(
	const std::string& _filename
) {

	std::ifstream infile(_filename);
	return load(infile);
}

void savegame_io::save_to_file(
	const std::string& _filename,
	const savegame& _data
) {

	std::ofstream outfile(_filename);
	return save(outfile, _data);
}

/**
 * generic load, the format is actually very simple: a word with the data node
 * name and the value as a string following it until a newline is found. 
 * because every single item must be in a simple line the persistence layer
 * has its newlines scaped on input.
 */
savegame savegame_io::load(
	std::istream& _stream
) {

	std::istringstream ss;
	savegame result=savegame::from_default();

	while(true) {

		std::string line;
		std::getline(_stream, line);
		if(_stream.eof()) {

			break;
		}

		ss.str(line);
		std::string type;
		ss>>type;

		//not all types will be checked, stuff can be malformed.
		if(type=="map_filename") {

			ss>>result.map_filename;
		}
		else if(type=="persistence") {

			std::string persistence;
			std::getline(ss, persistence); //This reads the rest of the line.
			result.persistence_string=tools::str_replace(tools::str_trim(persistence), "\\n", "\n");
		}
		else if(type=="entry_id") {

			ss>>result.entry_id;
		}
		else if(type=="difficulty_setting") {

			ss>>result.difficulty_setting;
		}
		else if(type=="elapsed_seconds") {

			ss>>result.elapsed_seconds;
		}
		else if(type=="lives") {

			ss>>result.lives;
		}
		else if(type=="yellow_keys") {

			ss>>result.yellow_keys;
		}
		else if(type=="blue_keys") {

			ss>>result.blue_keys;
		}
		else if(type=="red_keys") {

			ss>>result.red_keys;
		}
		else if(type=="green_keys") {

			ss>>result.green_keys;
		}
		else if(type=="white_keys") {

			ss>>result.white_keys;
		}
		else if(type=="ultimate") {

			ss>>result.ultimate;
		}
		else if(type=="treasure") {

			ss>>result.treasure;
		}
		else if(type=="discovered_rooms") {

			ss>>result.discovered_rooms;
		}

		ss.clear();
	}

	if(result.is_malformed()) {

		throw std::runtime_error("could not load, malformed save!");
	}

	return result;
}

/**
 * generic save. Same format as load.
 */
void savegame_io::save(
	std::ostream& _stream,
	const savegame& _data
) {

	//the whole persistence string must be in a single line, so...
	auto persistence_str=tools::str_replace(_data.persistence_string, "\n", "\\n");

	_stream<<"map_filename "<<_data.map_filename<<"\n"
		<<"persistence "<<persistence_str<<"\n"
		<<"entry_id "<<_data.entry_id<<"\n"
		<<"difficulty_setting "<<_data.difficulty_setting<<"\n"
		<<"elapsed_seconds "<<_data.elapsed_seconds<<"\n"
		<<"discovered_rooms "<<_data.discovered_rooms<<"\n"
		<<"lives "<<_data.lives<<"\n"
		<<"yellow_keys "<<_data.yellow_keys<<"\n"
		<<"blue_keys "<<_data.blue_keys<<"\n"
		<<"red_keys "<<_data.red_keys<<"\n"
		<<"green_keys "<<_data.green_keys<<"\n"
		<<"white_keys "<<_data.white_keys<<"\n"
		<<"ultimate "<<_data.ultimate<<"\n"
		<<"treasure "<<_data.treasure
		<<std::endl;
}

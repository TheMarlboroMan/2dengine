#include "app/savegame.h"
#include <tools/string_utils.h>

#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace app;

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
	savegame result{};

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

		throw std::runtime_error("malformed save!");
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
		<<"ultimate "<<_data.ultimate<<"\n"
		<<"treasure "<<_data.treasure
		<<std::endl;
}

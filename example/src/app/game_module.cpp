#include "app/game_module.h"
#include <tools/file_utils.h>
#include <stdexcept>
#include <sstream>

using namespace app;

game_module::game_module(
	const std::string& _module_path,
	lm::logger& _logger
):logger{_logger} {
	load(_module_path);
}

void game_module::load(
	const std::string& _module_path
) {

	auto check_exists=[](const std::string& _path, const std::string& _type) {

		if(!std::filesystem::exists(_path)) {

			std::stringstream ss;
			ss<<"the module "<<_type<<" "<<_path<<" does not exist or cannot be read";
			throw std::runtime_error(ss.str());
		}
	};

	lm::log(logger).debug()<<"reading module file "<<_module_path<<"\n";
	std::filesystem::path mod_file_path{_module_path};
	check_exists(mod_file_path, "definition file");

	parse_module_file(mod_file_path);

	//From the directory we can get a lot more stuff...
	auto mod_path=mod_file_path.parent_path();
	auto fs_maps_path=mod_path;
	fs_maps_path/="maps/";
	check_exists(fs_maps_path, "maps directory");
	maps_path=fs_maps_path;

	auto fs_automap_path=mod_path;
	fs_automap_path/="automap.txt";
	check_exists(fs_automap_path, "automap file");
	automap_path=fs_automap_path;
	
	auto fs_localization_path=mod_path;
	fs_localization_path/="localization/";
	check_exists(fs_localization_path, "localization directory");
	localization_path=fs_localization_path;
}

void game_module::parse_module_file(
	const std::string& _module_path
) {

	bool    with_savegame_affix{false},
	        with_treasure_total{false},
	        with_room_total{false},
	        with_start_room{false},
	        with_end_room{false};

	std::ifstream stream{_module_path};

	while(true) {

		std::string key, value;
		stream>>std::ws;
		if(stream.eof()) {

			break;
		}

		stream>>key;
		std::getline(stream >> std::ws, value); //discard whitespace from stream...

		lm::log(logger).debug()<<"got "<<key<<"="<<value<<" from module file\n";

		if(""==value) {

			std::stringstream ss;
			ss<<"value for '"<<key<<"' empty in module file, no values can be empty";
			throw std::runtime_error(ss.str());
		}

		if(key=="startmap") {

			with_start_room=true;
			startmap=value;
		}
		else if(key=="endmap_partial") {

			with_end_room=true;
			endmap=value;
		}
		else if(key=="savegame_name") {

			with_savegame_affix=true;
			savegame_name=value;
		}
		else if(key=="total_rooms") {

			with_room_total=true;
			total_rooms=std::stoi(value);
		}
		else if(key=="total_treasure") {

			with_treasure_total=true;
			total_treasure=std::stoi(value);
		}
		else {

			std::stringstream ss;
			ss<<"unknown module info '"<<key<<"'";
			throw std::runtime_error(ss.str());
		}
	}

	if(!with_savegame_affix) {

		throw std::runtime_error("missing savegame_name entry in module file");
	}

	if(!with_treasure_total) {

		throw std::runtime_error("missing total_treasure entry in module file");
	}

	if(!with_room_total) {

		throw std::runtime_error("missing total_rooms entry in module file");
	}

	if(!with_start_room) {

		throw std::runtime_error("missing startmap entry in module file");
	}

	if(!with_end_room) {

		throw std::runtime_error("missing endmap_partial entry in module file");
	}
}

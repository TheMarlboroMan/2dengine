#include "app/automap_reader.h"

#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace app;

automap automap_reader::read_map(
	const std::string& _filename
) {

	if(!std::filesystem::exists(_filename)) {

		std::stringstream ss;
		ss<<"unable to find file "<<_filename;
		throw std::runtime_error(ss.str());
	}

	infile=std::ifstream{_filename.c_str()};
	if(!infile.is_open()) {

		std::stringstream ss;
		ss<<"unable to open file "<<_filename;
		throw std::runtime_error(ss.str());
	}

	automap result{};
	std::string map_filename{};

	while(true) {

		char next=infile.get();

		if(infile.eof()) {

			break;
		}

		switch(next) {
			case '>':
				parse_area(result);
			break;
			case '*': {
				if(!result.areas.size()) {

					throw std::runtime_error("unexpected cell definition");
				}

				parse_cell(result.areas.back(), map_filename);
				auto id=result.areas.back().cells.back().id;
				result.file_to_id.insert(std::make_pair(map_filename, id));
				result.id_to_file.insert(std::make_pair(id, map_filename));
			}
			break;
			case ';':
				if(!result.areas.size()) {

					throw std::runtime_error("unexpected cell definition");
				}

				if(!result.areas.back().cells.size()) {

					throw std::runtime_error("unexpected cell definition");
				}

				parse_feature(result.areas.back().cells.back());
			break;
			case ' ':
			case '\n':
			case '\r':
				//Noop, ignore these, we are mixing << and get(), which is a bit
				//dangerous.
			break;
			default: {
				std::stringstream ss;
				ss<<"unexpected character in automap '"<<(char)next<<"' ["<<(int)next<<"] in "<<_filename;
				throw std::runtime_error(ss.str());
			}
		}
	}

	infile.close();
	return result;
}

void automap_reader::parse_area(
	automap& _map
) {

	int id{0};
	std::string localization_key{};
	bool must_draw_map;

	infile>>id>>localization_key>>must_draw_map;
	if(infile.fail()) {

		throw std::runtime_error("bad area definition");
	}

	_map.areas.push_back({id, localization_key, must_draw_map, {}});
}

void automap_reader::parse_cell(
	map_area& _area,
	std::string& _filename
) {

	int id{0}, x{0}, y{0}, w{0}, h{0};

	infile>>_filename>>id>>x>>y>>w>>h;

	if(infile.fail()) {

		throw std::runtime_error("bad cell definition");
	}

	_area.cells.push_back({id, x, y, w, h, {}});
}

void automap_reader::parse_feature(
	map_cell& _cell
) {

	int x{0}, y{0}, flags{0};

	infile>>x>>y>>flags;

	if(infile.fail()) {

		throw std::runtime_error("bad feature definition");
	}

	_cell.features.push_back({x, y, flags});
}


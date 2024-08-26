#include "app/automap.h"
#include <stdexcept>

using namespace app;

std::ostream& app::operator<<(
	std::ostream& _stream,
	const automap& _map
) {

	for(const auto& area : _map.areas) {

		_stream<<area;
	}

	return _stream;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const map_area& _area
) {

	_stream<<">"<<_area.id<<" "<<_area.localization_key<<std::endl;
	for(const auto& cell : _area.cells) {

		_stream<<cell;
	}

	return _stream;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const map_cell& _cell
) {

	_stream<<"*"<<_cell.id<<" "<<_cell.x<<" "<<_cell.y<<" "<<_cell.w<<" "<<_cell.h<<std::endl;
	for(const auto& feature : _cell.features) {

		_stream<<feature;
	}

	return _stream;
}

std::ostream& app::operator<<(
	std::ostream& _stream,
	const map_feature& _feature
) {

	_stream<<";"<<_feature.x<<" "<<_feature.y<<" "<<_feature.flags<<std::endl;
	return _stream;
}

const map_area& automap::find_area_by_map(
	const std::string& _filename
) const {

	return find_area_by_map_id(file_to_id.at(_filename));
}

const map_area& automap::find_area_by_map_id(
	int _id
) const {

	//TODO: Sure this can be better?
	for(const auto& area : areas) {

		if(area.has_map(_id)) {

			return area;
		}
	}

	throw std::runtime_error("could not find area for map id");
}

bool map_area::has_map(
	int _id
) const {

	for(const auto& cell : cells) {

		if(cell.id==_id) {

			return true;
		}
	}
	return false;
}


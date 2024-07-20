#include "app/automap.h"

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

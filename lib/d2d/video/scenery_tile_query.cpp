#include "d2d/video/scenery_tile_query.h"
#include <tools/text_reader.h>
#include <tools/string_utils.h>
#include <stdexcept>

using namespace d2d::video;

scenery_tile_query::scenery_tile_query(
	const std::string& _path
) {

	tools::text_reader reader(_path);
	if(!reader) {

		throw std::runtime_error("failed to open tile to animation file");
	}

	std::string tmp;
	while(true) {

		tmp=reader.read_line();
		if(0==tmp.size()) {

			break;
		}

		//TODO: use a stringstream???
		auto pieces=tools::explode(tmp, ' ');
		if(2!=pieces.size()) {

			throw std::runtime_error("invalid line for tile to animation file");
		}

		int tile=std::stoi(pieces[0]);
		int animation=std::stoi(pieces[1]);
		tile_to_animation[tile]=animation;
	}
}

bool scenery_tile_query::is_animation(
	int _index
) const {

	return tile_to_animation.count(_index);
}

int scenery_tile_query::get_animation(
	int _index
) const {

	return tile_to_animation.at(_index);
}


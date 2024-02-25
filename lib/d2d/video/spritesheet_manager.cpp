#include "d2d/video/spritesheet_manager.h"
#include "d2d/video/exception.h"
#include <sstream>

using namespace d2d::video;

spritesheet_manager& spritesheet_manager::add(
	int _index,
	const ldtools::sprite_table& _table
) {

	if(container.count(_index)) {

		std::stringstream ss;
		ss<<"no sprite table could be found at index "<<_index;
		throw exception(ss.str());
	}

	container[_index]=_table;

	return *this;
}

const ldtools::sprite_table& spritesheet_manager::at(
	int _index
) const {

	if(!container.count(_index)) {

		std::stringstream ss;
		ss<<"no sprite table could be found at index "<<_index;
		throw exception(ss.str());
	}

	return container.at(_index);
}

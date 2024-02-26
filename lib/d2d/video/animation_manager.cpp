#include "d2d/video/animation_manager.h"
#include "d2d/video/exception.h"
#include <sstream>

using namespace d2d::video;

animation_manager& animation_manager::add(
	int _index,
	const ldtools::animation_table& _table
) {

	if(container.count(_index)) {

		std::stringstream ss;
		ss<<"no animation table could be found at index "<<_index;
		throw exception(ss.str());
	}

	container.insert(std::make_pair(_index, _table));

	return *this;
}

const ldtools::animation_table& animation_manager::at(
	int _index
) const {

	if(!container.count(_index)) {

		std::stringstream ss;
		ss<<"no animation table could be found at index "<<_index;
		throw exception(ss.str());
	}

	return container.at(_index);
}

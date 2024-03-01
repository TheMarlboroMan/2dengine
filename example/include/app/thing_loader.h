#pragma once

#include "map.h"
#include <d2d/storage/thing_processor.h>

namespace app {

class thing_loader
	: public d2d::storage::thing_processor {

	public:

						thing_loader(map&);

	void                setup();
	void                load(d2d::storage::position, int, const std::map<std::string, d2d::storage::attribute>&);

	private:

	map&                       curmap;
};
}

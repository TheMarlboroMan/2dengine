#pragma once

#include "d2d/storage/property_processor.h"

#include <ldv/color.h>

namespace app {

class map_attribute_loader:
	public d2d::storage::property_processor {

	public:

	                            map_attribute_loader(ldv::rgba_color&, int&, int&, bool&);

	ldv::rgba_color&            background_color;
	int&                        music_id;
	int&                        background_effect;
	bool&                       in_game;

//begin implementation of base class
	virtual void                setup();
	virtual void                load(const std::map<std::string, d2d::storage::attribute>&);
//end implementation of base classe.
};
}

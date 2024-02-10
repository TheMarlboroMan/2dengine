#pragma once

#include "entity.h"
#include <iostream>

namespace app {

class solid_block:
	public entity {

	public:

	                                    solid_block(int, int, int, int);

	virtual d2d::collision::color       get_debug_outline_color() const {return ldv::rgba8(255,0,64,255);}
	virtual d2d::collision::color       get_debug_fill_color() const {return ldv::rgba8(128, 0, 64, 255);}
};

std::ostream& operator<<(std::ostream&, const solid_block&);
}

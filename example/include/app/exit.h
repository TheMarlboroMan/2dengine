#pragma once

#include "entity.h"
#include <ostream>
#include <string>

namespace app {

class exit {

	public:

	                        exit(d2d::collision::box, const std::string&, int, int, bool);

	entity                  ent;
	std::string             map_filename;
	int                     next_entry_id,
	                        min_rooms;
	bool                    touch;
};

std::ostream& operator<<(std::ostream&, const exit&);
}

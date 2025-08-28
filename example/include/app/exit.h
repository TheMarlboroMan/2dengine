#pragma once

#include "entity.h"
#include <ostream>
#include <string>

namespace app {

class exit {

	public:

	enum transitions {
		transition_none=0,
		transition_fade_to_black=1,
		transition_fade_to_white=2,
	};

	                        exit(d2d::collision::box, const std::string&, int, int, int, bool);

	entity                  ent;
	std::string             map_filename;
	int                     next_entry_id,
	                        min_rooms, //minimum rooms visited to activate this exit.
	                        transition_type; //transition to next map according to enum above.
	bool                    touch; //whether or not this exit is activated by touch or "action".
};

std::ostream& operator<<(std::ostream&, const exit&);
}

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

	enum types {
		trigger=0,
		touch=1,
		redkey=2,
		bluekey=3,
		greenkey=4
	};

	                        exit(d2d::collision::box, const std::string&, int, int, int, int);
	//All exits are "touch" except for trigger ones.
	bool                    is_touch() const {return type!=trigger;}
	bool                    is_special() const {return type >= redkey;}
	bool                    operator!=(const exit& _other) const {return !(*this==_other);}
	bool                    operator==(const exit&) const;

	entity                  ent;
	std::string             map_filename;
	int                     next_entry_id,
	                        min_rooms, //minimum rooms visited to activate this exit.
	                        transition_type, //transition to next map according to enum above.
	                        type; //exit type.
};

std::ostream& operator<<(std::ostream&, const exit&);
}

#pragma once

#include "entity.h"
#include <iostream>

namespace app {

class touch_trigger {

	public:

	                    touch_trigger(const d2d::collision::box&, int, int, bool, bool);
	void                reset();
	bool                is_keep_used_when_reset() const {return keep_used_when_reset;}

	entity                          ent;
	int                             id;
	int                             tag;
	bool                            used;


	private:

	bool                            keep_used_when_reset;
};

std::ostream& operator<<(std::ostream&, const touch_trigger&);
}

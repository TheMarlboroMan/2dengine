#pragma once

#include "entity.h"
#include <ostream>

namespace app {

class collectible {

	public:

	enum types {
		gold_ingot=0,
		gem=1,
		ruby=2,
		yellow_key=10,
		blue_key=11,
		red_key=12
	};

/*
 * arguments are point, id and type
 */
									collectible(d2d::collision::point, int, int);
	int                             id; //The identifier is automatic.
	int                             type;
	app::entity                     ent;

	private:

	static const int                gold_ingot_w{16};
	static const int                gold_ingot_h{9};
	static const int                gem_w{10};
	static const int                gem_h{14};
	static const int                ruby_w{10};
	static const int                ruby_h{10};
	static const int                key_w{16};
	static const int                key_h{10};
};

std::ostream& operator<<(std::ostream&, const collectible&);
}

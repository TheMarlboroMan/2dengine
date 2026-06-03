#pragma once

#include "entity.h"
#include <ostream>

namespace app {

class collectible {

	public:

	//TODO: Make it an enum class.
	enum types {
		gold_ingot=0,
		gem=1,
		ruby=2,
		diamond=3,
		yellow_key=10,
		blue_key=11,
		red_key=12,
		green_key=13,
		ultimate=14,
		white_key=15
	};

/*
 * arguments are point, id and type
 */
									collectible(d2d::collision::point, int, int);
	int                             id; //The identifier is automatic.
	int                             type;
	app::entity                     ent;
	void                            mark_to_remove() {to_be_removed=true;}
	bool                            is_to_be_removed() const {return to_be_removed;}

	private:

	bool                            to_be_removed{false};

	static const int                gold_ingot_w{16};
	static const int                gold_ingot_h{9};
	static const int                gem_w{10};
	static const int                gem_h{14};
	static const int                ruby_w{10};
	static const int                ruby_h{10};
	static const int                diamond_w{15};
	static const int                diamond_h{13};
	static const int                key_w{16};
	static const int                key_h{10};
};

std::ostream& operator<<(std::ostream&, const collectible&);
}

#include "d2d/collision/aabb_checker.h"
#include "d2d/collision/exception.h"
#include <iostream>

using namespace d2d::collision;

bool aabb_checker::check(
	const d2d::collision::spatiable& _subject,
	const d2d::collision::spatiable& _obstacle,
	phases _phase,
	int _flags
) const {

	if(!d2d::collision::collides_with(_obstacle, _subject)) {

		return false;
	}

	if(_flags & flag_skip_passable_side_check) {

		return true;
	}

	switch(_phase) {

		case phases::horizontal: {

			auto edge=d2d::collision::is_right_of(_obstacle, _subject.get_previous_box())
				? box_edge::left
				: box_edge::right;

			return !_obstacle.is_passable_edge(edge);
		}
		break;
		case phases::vertical: {

			auto edge=d2d::collision::is_below(_obstacle, _subject.get_previous_box())
				? box_edge::top
				: box_edge::bottom;

			return !_obstacle.is_passable_edge(edge);
		}
		break;
	}

	return false; //never gonna happen.
}


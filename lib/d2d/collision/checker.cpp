#include "d2d/collision/checker.h"
#include "d2d/collision/exception.h"
#include <iostream>

using namespace d2d::collision;

bool checker::check(
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

/*
void checker::start(
	const d2d::collision::box& _box,
	phases _phase
) {

	results.clear();
	phase=_phase;
	subject=_box;
	previous_position=_box;
	started=true;
}

void checker::start(
	const d2d::collision::spatiable& _subject,
	phases _phase
) {

	results.clear();
	phase=_phase;
	subject=_subject.get_box();
	previous_position=_subject.get_previous_box();
	started=true;
}

bool checker::add(
	const d2d::collision::spatiable& _obstacle,
	int _flags
) {

	if(!started) {

		throw exception("cannot call checker::add before start");
	}

	if(collides_with(_obstacle, subject)) {

		if(_flags & flag_skip_passable_side_check) {

			results.push_back(&_obstacle);
			return true;
		}

		switch(phase) {

			case phases::horizontal: {

				auto edge=is_right_of(_obstacle, previous_position)
					? box_edge::left
					: box_edge::right;

				if(!_obstacle.is_passable_edge(edge)) {

					results.push_back(&_obstacle);
					return true;
				}
			}
			break;
			case phases::vertical: {

				auto edge=is_below(_obstacle, previous_position)
					? box_edge::top
					: box_edge::bottom;

				if(!_obstacle.is_passable_edge(edge)) {

					results.push_back(&_obstacle);
					return true;
				}
			}
			break;
		}
	}

	return false;
}

std::vector<spatiable const *> checker::end() {

	if(!started) {

		throw exception("cannot call checker::end before start");
	}

	started=false;
	return results;
}
*/

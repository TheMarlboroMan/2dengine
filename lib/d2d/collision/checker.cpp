#include "d2d/collision/checker.h"
#include "d2d/collision/tools.h"
#include <stdexcept>
#include <iostream>

using namespace d2d::collision;

std::vector<spatiable const * > checker::get_collisions(
	const collision::box& _box,
	const std::vector<spatiable>& _boxes
) {

	std::vector<spatiable const*> result{};

	for(const auto& obstacle : _boxes) {

		if(collides_with(obstacle, _box)) {
			result.push_back(&obstacle);
		}
	}

	return result;
}

std::vector<spatiable const *> checker::get_collisions(
	const box& _box,
	const std::vector<spatiable const*>& _boxes
) {

	std::vector<spatiable const*> result{};

	for(const auto& obstacle : _boxes) {
		if(collides_with(*obstacle, _box)) {
			result.push_back(obstacle);
		}
	}

	return result;
}

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

		throw std::runtime_error("cannot call checker::add before start");
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

		throw std::runtime_error("cannot call checker::end before start");
	}

	started=false;
	return results;
}

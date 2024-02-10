#include "d2d/collision/checker.h"
#include "d2d/collision/tools.h"
#include <stdexcept>

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
	const d2d::collision::box& _box
) {

	subject=_box;
	results.clear();
	started=true;
}

void checker::start(
	const d2d::collision::spatiable& _subject
) {

	start(_subject.get_box());
}

bool checker::add(
	const d2d::collision::spatiable& _obstacle
) {

	if(!started) {

		throw std::runtime_error("cannot call checker::add before start");
	}

	if(collides_with(_obstacle, subject)) {
		results.push_back(&_obstacle);
		return true;
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

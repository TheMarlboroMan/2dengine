#include "d2d/collision/checker.h"
#include "d2d/collision/tools.h"

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

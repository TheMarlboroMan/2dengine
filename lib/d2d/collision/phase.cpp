#include <d2d/collision/phase.h>
#include <d2d/collision/solver.h>

using namespace d2d::collision;

phase::phase(
	d2d::collision::spatiable& _subject,
	d2d::collision::checker::phases _phase
):
	collision_phase{_phase},
	subject{_subject}
{ }

bool phase::detect_one(
	const d2d::collision::spatiable& _node, 
	int _flags
) {

	if(checker.check(subject, _node, collision_phase, _flags)) {

		collision_found=true;
		results.push_back(&_node);
		return true;
	}

	return false;
}

void phase::response_generic() {

	if(!collision_found) {

		return;
	}

	auto collision_solver=d2d::collision::solver{};

	switch(collision_phase) {
		case d2d::collision::checker::phases::horizontal:
			collision_solver.horizontal(subject, results);
			return;
		case d2d::collision::checker::phases::vertical:
			collision_solver.vertical(subject, results);
			return;
	}
}

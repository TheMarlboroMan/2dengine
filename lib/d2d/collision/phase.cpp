#include "d2d/collision/phase.h"
#include "d2d/collision/exception.h"

using namespace d2d::collision;

phase::phase(
	d2d::collision::spatiable& _subject,
	d2d::collision::checker::phases _phase
):
	collision_phase{_phase},
	subject{_subject}
{ }

phase& phase::reset_modifiers() {

	collision_flags=0;
	with_early_exit=false;
	return *this;
}

phase& phase::flags(
	int _val
) {
	collision_flags=_val;
	return *this;
}

phase& phase::early_exit(
	bool _val
) {

	with_early_exit=_val;
	return *this;
}


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

d2d::collision::response phase::response_complex() {

	if(!collision_found) {

		throw exception("cannot get complex response where no collisions exist");
	}

	auto collision_solver=d2d::collision::solver{};

	switch(collision_phase) {
		case d2d::collision::checker::phases::horizontal:
			return collision_solver.horizontal_complex(subject, results);
		case d2d::collision::checker::phases::vertical:
			return collision_solver.vertical_complex(subject, results);
	}

	throw exception("shut up compiler");
}

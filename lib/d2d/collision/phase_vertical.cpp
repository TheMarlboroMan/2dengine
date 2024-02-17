#include <d2d/collision/phase_vertical.h>
#include <d2d/collision/solver.h>

using namespace d2d::collision;

phase_vertical::phase_vertical(
	d2d::collision::spatiable& _subject
):
	subject{_subject}
{

	checker.start(subject, d2d::collision::checker::phases::vertical);
}

bool phase_vertical::detect_one(
	const d2d::collision::spatiable& _node, 
	int _flags
) {

	bool result=checker.add(_node, _flags);
	collision_found|=result;
	return result;
}

void phase_vertical::response_generic() {

	if(!collision_found) {

		return;
	}

	auto collision_solver=d2d::collision::solver{};
	collision_solver.vertical(subject, checker.end());
}

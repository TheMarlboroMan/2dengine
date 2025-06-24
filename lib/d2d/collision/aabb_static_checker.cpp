#include "d2d/collision/aabb_static_checker.h"
#include "d2d/collision/tools.h"

using namespace d2d::collision;

aabb_static_checker::aabb_static_checker(
	const d2d::collision::spatiable& _subject,
	bool _with_early_exit
):
	with_early_exit{_with_early_exit},
	subject{_subject.get_box()}
{ }

aabb_static_checker::aabb_static_checker(
	const d2d::collision::box& _box,
	bool _with_early_exit
):
	with_early_exit{_with_early_exit},
	subject{_box}
{ }

aabb_static_checker& aabb_static_checker::reset() {

	results.clear();
	collision_found=false;
	return *this;
}

aabb_static_checker& aabb_static_checker::set_early_exit(
	bool _val
) {

	with_early_exit=_val;
	return *this;
}


aabb_static_checker& aabb_static_checker::detect_one(
	const d2d::collision::spatiable& _node
) {

	if(collides_with(subject, _node)) {

		collision_found=true;
		results.push_back(&_node);
	}

	return *this;
}

aabb_static_checker& aabb_static_checker::set_subject(
	const d2d::collision::spatiable& _subject
) {

	subject=_subject.get_box();
	return *this;
}

aabb_static_checker& aabb_static_checker::set_subject(
	const d2d::collision::box& _subject
) {

	subject=_subject;
	return *this;
}

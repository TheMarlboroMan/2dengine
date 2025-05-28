#include "d2d/collision/aabb_static_checker.h"
#include "d2d/collision/tools.h"

using namespace d2d::collision;

aabb_static_checker::aabb_static_checker(
	const d2d::collision::spatiable& _subject
):
	subject{_subject.get_box()}
{ }

aabb_static_checker::aabb_static_checker(
	const d2d::collision::box& _box
):
	subject{_box}
{ }

aabb_static_checker& aabb_static_checker::reset() {

	results.clear();
	return *this;
}

aabb_static_checker& aabb_static_checker::reset_modifiers() {

	with_early_exit=false;
	return *this;
}

aabb_static_checker& aabb_static_checker::early_exit(
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

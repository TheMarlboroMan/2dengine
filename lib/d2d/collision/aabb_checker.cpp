#include "d2d/collision/aabb_checker.h"

using namespace d2d::collision;

aabb_checker::aabb_checker(
	const d2d::collision::spatiable& _subject
)
	:subject{_subject.get_box()}
{}

aabb_checker::aabb_checker(
	const d2d::collision::box& _subject
)
	:subject{_subject}
{}

aabb_checker& aabb_checker::set_subject(
	const d2d::collision::spatiable& _subject
) {

	subject=_subject.get_box();
	return *this;
}

aabb_checker& aabb_checker::set_subject(
	const d2d::collision::box& _subject
) {

	subject=_subject;
	return *this;
}

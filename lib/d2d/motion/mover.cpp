#include <d2d/motion/mover.h>

using namespace d2d::motion;

void mover::apply(
	d2d::collision::spatiable& _subject,
	const d2d::motion::motion_vector& _vector,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	box.origin.x+=_vector.x * _delta;
	box.origin.y+=_vector.y * _delta;
}

void mover::apply_and_commit(
	d2d::collision::spatiable& _subject,
	const d2d::motion::motion_vector& _vector,
	ldtools::tdelta _delta
) {

	apply(_subject, _vector, _delta);
	_subject.commit_box();
}

void mover::apply_x(
	d2d::collision::spatiable& _subject,
	double _force,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	box.origin.x+=_force * _delta;
}

void mover::apply_y(
	d2d::collision::spatiable& _subject,
	double _force,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	box.origin.y+=_force * _delta;
}

void mover::apply(
	d2d::collision::spatiable& _subject,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	auto vector=_subject.get_motion_vector();
	box.origin.x+=vector.x * _delta;
	box.origin.y+=vector.y * _delta;
}

void mover::apply_and_commit(
	d2d::collision::spatiable& _subject,
	ldtools::tdelta _delta
) {

	apply(_subject, _delta);
	_subject.commit_box();
}

void mover::apply_x(
	d2d::collision::spatiable& _subject,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	box.origin.x+=_subject.get_motion_vector_x() * _delta;
}

void mover::apply_y(
	d2d::collision::spatiable& _subject,
	ldtools::tdelta _delta
) {

	auto& box=_subject.get_box();
	box.origin.y+=_subject.get_motion_vector_y() * _delta;
}

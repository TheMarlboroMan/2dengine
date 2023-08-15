#include <d2d/motion/mover.h>

using namespace d2d::motion;

void mover::apply(
	d2d::collision::spatiable& _subject,
	const d2d::motion::motion_vector& _vector,
	float _delta
) {

	auto& box=_subject.get_box();
	box.origin.x+=_vector.x * _delta;
	box.origin.y+=_vector.y * _delta;
}

void mover::apply_x(
	d2d::collision::spatiable& _subject,
	double _force,
	float _delta
) {

	auto& box=_subject.get_box();
	box.origin.x+=_force * _delta;
}

void mover::apply_y(
	d2d::collision::spatiable& _subject,
	double _force,
	float _delta
) {

	auto& box=_subject.get_box();
	box.origin.y+=_force * _delta;
}

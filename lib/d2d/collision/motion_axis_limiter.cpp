#include <d2d/collision/motion_axis_limiter.h>
#include <d2d/collision/tools.h>

using namespace d2d::collision;

motion_axis_limiter::motion_axis_limiter(
	const d2d::collision::box& _box,
	motion_axis_limiter::axes _axis, 
	motion_axis_limiter::alignments _alignment
):
	collision_box(_box),
	axis{_axis},
	alignment{_alignment}
{}

void motion_axis_limiter::apply(
	spatiable& _target
) const {

	using namespace d2d::collision;

	//This is one ugly table right here...
	switch(axis) {

		case motion_axis_limiter::axes::x:

			if(is_left_of(_target, *this)) {

				match_left_of(_target, *this);
			}
			else if(is_right_of(_target, *this)) {

				match_right_of(_target, *this);
			}

			switch(alignment) {

				case motion_axis_limiter::alignments::near:

					match_bottom_of(_target, *this);
				break;
				case motion_axis_limiter::alignments::center:

					center_vertically(_target, *this);
				break;
				case motion_axis_limiter::alignments::far:

					match_top_of(_target, *this);
				break;
			}
		break;
		case motion_axis_limiter::axes::y:

			if(is_below(_target, *this)) {

				match_bottom_of(_target, *this);
			}
			else if(is_above(_target, *this)) {

				match_top_of(_target, *this);
			}

			switch(alignment) {

				case motion_axis_limiter::alignments::near:

					match_left_of(_target, *this);
				break;
				case motion_axis_limiter::alignments::center:

					center_horizontally(_target, *this);
				break;
				case motion_axis_limiter::alignments::far:

					match_right_of(_target, *this);
				break;
			}
		break;
	}
}

motion_axis_limiter& motion_axis_limiter::set_axis(
	motion_axis_limiter::axes _axis
) {

	axis=_axis;
	return *this;
}

motion_axis_limiter& motion_axis_limiter::set_alignment(
	motion_axis_limiter::alignments _alignment
) {

	alignment=_alignment;
	return *this;
}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream, 
	const motion_axis_limiter::axes& _axis
) {

	_stream<<(_axis==motion_axis_limiter::axes::x ? 'x' : 'y');
	return _stream;
}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream, 
	const motion_axis_limiter::alignments& _alignment
) {

	switch(_alignment) {

		case motion_axis_limiter::alignments::near: _stream<<"near"; break;
		case motion_axis_limiter::alignments::center: _stream<<"center"; break;
		case motion_axis_limiter::alignments::far: _stream<<"far"; break;
	}

	return _stream;
}

std::ostream& d2d::collision::operator<<(
	std::ostream& _stream, 
	const motion_axis_limiter& _limiter
) {

	_stream<<"limiter["<<_limiter.get_box()<<", "<<_limiter.get_axis()<<","<<_limiter.get_alignment()<<"]";

	return _stream;
}





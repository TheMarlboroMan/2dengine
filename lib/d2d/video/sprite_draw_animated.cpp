#include "d2d/video/sprite_draw_animated.h"

using namespace d2d::video;

sprite_draw_animated::sprite_draw_animated(
	const ldtools::animation_table& _animation_table
):
	animation_table{&_animation_table}
{}

void sprite_draw_animated::tic(
	float _delta
) {

	internal_timer+=_delta;
}

sprite_draw_animated& sprite_draw_animated::reset_animation(
	const ldtools::animation_table& _table
) {

	animation_table=&_table;
	return *this;
}

const ldtools::animation_line& sprite_draw_animated::get(
	const ldtools::animation& _animation
) const {

	return get(_animation, internal_timer);
}

const ldtools::animation_line& sprite_draw_animated::get(
	int _animation_index
) const {

	const auto& animation=animation_table->get(_animation_index);
	return get(animation, internal_timer);
}

const ldtools::animation_line& sprite_draw_animated::get(
	const ldtools::animation& _animation,
	float _timer
) const {

	return _animation.get_for_time(_timer);
}

const ldtools::animation_line& sprite_draw_animated::get(
	int _animation_index,
	float _timer
) const {

	const auto& animation=animation_table->get(_animation_index);
	return get(animation, _timer);
}

/**
* get frame line from animation using the given float time if the animation
* lasted that other float time.
*/
const ldtools::animation_line& sprite_draw_animated::get(
	const ldtools::animation& _animation,
	float _timer,
	float _length
) const {

	return _animation.get_for_time(_timer, _length);
}

const ldtools::animation_line& sprite_draw_animated::get(
	int _animation_index,
	float _timer,
	float _length
) const {

	const auto& animation=animation_table->get(_animation_index);
	return get(animation, _timer, _length);
}


sprite_draw::flags sprite_draw_animated::flags(
	const ldtools::animation_line& _line
) const {

	return flags(_line, sprite_draw::flags{});
}

sprite_draw::flags sprite_draw_animated::flags(
	const ldtools::animation_line& _line,
	sprite_draw::flags _flags
) const {

	//The animation may ask for the frame to be flipped.
	if(_line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(_line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(_line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(_line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	int degrees=_line.get_rotation() + _line.frame.get_rotation() + _flags.rotation_degrees;
	_flags.rotation_degrees=degrees % 360;

	return _flags;
}

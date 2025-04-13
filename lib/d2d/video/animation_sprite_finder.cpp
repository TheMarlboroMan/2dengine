#include "d2d/video/animation_sprite_finder.h"

using namespace d2d::video;

animation_sprite_finder::animation_sprite_finder(
	const ldtools::animation_table& _animation_table
):
	animation_table{&_animation_table}
{}

void animation_sprite_finder::tic(
	float _delta
) {

	internal_timer+=_delta;
}

animation_sprite_finder& animation_sprite_finder::reset_table(
	const ldtools::animation_table& _table
) {

	animation_table=&_table;
	return *this;
}

const ldtools::animation_line& animation_sprite_finder::get(
	const ldtools::animation& _animation
) const {

	return get(_animation, internal_timer);
}

const ldtools::animation_line& animation_sprite_finder::get(
	int _animation_index
) const {

	const auto& animation=animation_table->get(_animation_index);
	return get(animation, internal_timer);
}

const ldtools::animation_line& animation_sprite_finder::get(
	const ldtools::animation& _animation,
	float _timer
) const {

	return _animation.get_for_time(_timer);
}

const ldtools::animation_line& animation_sprite_finder::get(
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
const ldtools::animation_line& animation_sprite_finder::get(
	const ldtools::animation& _animation,
	float _timer,
	float _length
) const {

	return _animation.get_for_time(_timer, _length);
}

const ldtools::animation_line& animation_sprite_finder::get(
	int _animation_index,
	float _timer,
	float _length
) const {

	const auto& animation=animation_table->get(_animation_index);
	return get(animation, _timer, _length);
}


sprite_draw::modifiers animation_sprite_finder::modifiers(
	const ldtools::animation_line& _line
) const {

	return modifiers(_line, sprite_draw::modifiers{});
}

sprite_draw::modifiers animation_sprite_finder::modifiers(
	const ldtools::animation_line& _line,
	sprite_draw::modifiers _modifiers
) const {


	//The animation may ask for the frame to be flipped.
	if(_line.is_flipped_horizontally()) {

		_modifiers.flags ^= sprite_draw::modifiers::flip_horizontal;
	}

	if(_line.is_flipped_vertically()) {

		_modifiers.flags ^= sprite_draw::modifiers::flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(_line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_modifiers.flags ^= sprite_draw::modifiers::flip_horizontal;
	}

	if(_line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_modifiers.flags ^= sprite_draw::modifiers::flip_vertical;
	}

	int degrees=_line.get_rotation() + _line.frame.get_rotation() + _modifiers.rotation_degrees;
	_modifiers.rotation_degrees=degrees % 360;

	return _modifiers;
}

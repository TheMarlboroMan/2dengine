#include "d2d/video/sprite_draw_animated.h"

using namespace d2d::video;

sprite_draw_animated::sprite_draw_animated(
	const ldtools::sprite_table& _table,
	const ldv::texture& _texture,
	const ldtools::animation_table& _animation_table
):
	spr_draw{_table, _texture},
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

void sprite_draw_animated::draw_frame(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	const ldtools::animation& _animation,
	int _frame_index,
	sprite_draw::flags _flags
) {

	//TODO: This could actually be the end for the draw() line of methods too...
	const auto& line=_animation.get(_frame_index);

	//The animation may ask for the frame to be flipped.
	if(line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	spr_draw.draw(_screen, _camera, _point, line.frame, _flags);
}

void sprite_draw_animated::draw_frame(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	const ldtools::animation& _animation,
	int _frame_index
) {

	draw_frame(_screen, _camera, _point, _animation, _frame_index, {false, false});
}

void sprite_draw_animated::draw_frame(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _animation_index, 
	int _frame_index,
	sprite_draw::flags _flags
) {

	const auto& animation=animation_table->get(_animation_index);
	draw_frame(_screen, _camera, _point, animation, _frame_index, _flags);
}

void sprite_draw_animated::draw_frame(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _animation_index, 
	int _frame_index
) {

	draw_frame(_screen, _camera, _point, _animation_index, _frame_index, {false, false});
}

//with point, animation and camera.
void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	ldv::point _point,
	const ldtools::animation& _animation,
	float _timer,
	sprite_draw::flags _flags
) {

	const auto& line=_animation.get_for_time(_timer);

	//The animation may ask for the frame to be flipped.
	if(line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	spr_draw.draw(_screen, _camera, _point, line.frame, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	const ldtools::animation& _animation,
	float _timer
) {

	draw(_screen, _camera, _point, _animation, _timer, {false, false});
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	const ldtools::animation& _animation,
	sprite_draw::flags _flags
) {

	draw(_screen, _camera, _point, _animation, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	const ldtools::animation& _animation
) {

	draw(_screen, _camera, _point, _animation, internal_timer);
}

//with point, index and camera
void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	ldv::point _point,
	int _index,
	float _timer,
	sprite_draw::flags _flags
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _camera, _point, animation, _timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _index,
	float _timer
) {

	draw(_screen, _camera, _point, _index, _timer, {false, false});
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _index,
	sprite_draw::flags _flags
) {

	draw(_screen, _camera, _point, _index, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _point, 
	int _index
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _camera, _point, animation, internal_timer);
}

//with point, animation and NO camera.

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _point, 
	const ldtools::animation& _animation,
	float _timer, 
	sprite_draw::flags _flags
) {

	const auto& line=_animation.get_for_time(_timer);

	//The animation may ask for the frame to be flipped.
	if(line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	spr_draw.draw(_screen, _point, line.frame, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	const ldtools::animation& _animation,
	sprite_draw::flags _flags
) {

	draw(_screen, _point, _animation, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	const ldtools::animation& _animation
) {

	draw(_screen, _point, _animation, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _point, 
	const ldtools::animation& _animation,
	float _timer
) {

	draw(_screen, _point, _animation, _timer, {false, false});
}

//with point, index and NO camera.

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _point, 
	int _index,
	float _timer, 
	sprite_draw::flags _flags
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _point, animation, _timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	int _index,
	sprite_draw::flags _flags
) {

	draw(_screen, _point, _index, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::point _point, 
	int _index
) {

	draw(_screen, _point, _index, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _point, 
	int _index,
	float _timer
) {

	draw(_screen, _point, _index, _timer, {false, false});
}

//The same again, but with RECTS.
//with point, animation and camera.
void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	ldv::rect _rect,
	const ldtools::animation& _animation,
	float _timer,
	sprite_draw::flags _flags
) {

	const auto& line=_animation.get_for_time(_timer);

	//The animation may ask for the frame to be flipped.
	if(line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	spr_draw.draw(_screen, _camera, _rect, line.frame, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	const ldtools::animation& _animation,
	float _timer
) {

	draw(_screen, _camera, _rect, _animation, _timer, {false, false});
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	const ldtools::animation& _animation,
	sprite_draw::flags _flags
) {

	draw(_screen, _camera, _rect, _animation, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	const ldtools::animation& _animation
) {

	draw(_screen, _camera, _rect, _animation, internal_timer);
}

//with rect, index and camera
void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	ldv::rect _rect,
	int _index,
	float _timer,
	sprite_draw::flags _flags
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _camera, _rect, animation, _timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	int _index,
	float _timer
) {

	draw(_screen, _camera, _rect, _index, _timer, {false, false});
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	int _index,
	sprite_draw::flags _flags
) {

	draw(_screen, _camera, _rect, _index, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::rect _rect, 
	int _index
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _camera, _rect, animation, internal_timer);
}

//with rect, animation and NO camera.

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::rect _rect, 
	const ldtools::animation& _animation,
	float _timer, 
	sprite_draw::flags _flags
) {

	const auto& line=_animation.get_for_time(_timer);

	//The animation may ask for the frame to be flipped.
	if(line.is_flipped_horizontally()) {

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.is_flipped_vertically()) {

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	//The frame ITSELF might be flipped too xD
	if(line.frame.flags & ldtools::sprite_frame::horizontal_flip) { 

		_flags.flip_horizontal=!_flags.flip_horizontal;
	}

	if(line.frame.flags & ldtools::sprite_frame::vertical_flip) { 

		_flags.flip_vertical=!_flags.flip_vertical;
	}

	spr_draw.draw(_screen, _rect, line.frame, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	const ldtools::animation& _animation,
	sprite_draw::flags _flags
) {

	draw(_screen, _rect, _animation, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	const ldtools::animation& _animation
) {

	draw(_screen, _rect, _animation, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::rect _rect, 
	const ldtools::animation& _animation,
	float _timer
) {

	draw(_screen, _rect, _animation, _timer, {false, false});
}

//with rect, index and NO camera.

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::rect _rect, 
	int _index,
	float _timer, 
	sprite_draw::flags _flags
) {

	const auto& animation=animation_table->get(_index);
	draw(_screen, _rect, animation, _timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	int _index,
	sprite_draw::flags _flags
) {

	draw(_screen, _rect, _index, internal_timer, _flags);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	ldv::rect _rect, 
	int _index
) {

	draw(_screen, _rect, _index, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::rect _rect, 
	int _index,
	float _timer
) {

	draw(_screen, _rect, _index, _timer, {false, false});
}

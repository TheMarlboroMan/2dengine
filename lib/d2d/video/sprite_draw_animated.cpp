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

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera, 
	ldv::point _pt, 
	const ldtools::animation& _animation
) {

	draw(_screen, _camera, _pt, _animation, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _pt,
	const ldtools::animation& _animation
) {

	draw(_screen, _pt, _animation, internal_timer);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	ldv::point _pt,
	const ldtools::animation& _animation,
	float _timer
) {

	//TODO: What about the thing being flipped and stuff???
	const auto& line=_animation.get_for_time(_timer);
	spr_draw.draw(_screen, _camera, _pt, line.frame);
}

void sprite_draw_animated::draw(
	ldv::screen& _screen,
	ldv::point _pt, 
	const ldtools::animation& _animation,
	float _timer
) {

	//TODO: What about the thing being flipped and stuff???
	const auto& line=_animation.get_for_time(_timer);
	spr_draw.draw(_screen, _pt, line.frame);
}


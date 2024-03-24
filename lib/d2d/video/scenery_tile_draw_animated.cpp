#include "d2d/video/scenery_tile_draw_animated.h"
#include "d2d/video/exception.h"
#include <iostream>

using namespace d2d::video;

scenery_tile_draw_animated::scenery_tile_draw_animated(
	const ldtools::sprite_table& _sprite_table, 
	const ldv::texture& _texture,
	int _tile_w,
	int _tile_h,
	const ldtools::animation_table& _animation_table,
	ldv::camera * _camera,
	bool _with_camera
):
	static_draw{_sprite_table, _texture, _tile_w, _tile_h, _camera, _with_camera},
	animation_table{&_animation_table}
{ }

scenery_tile_draw_animated& scenery_tile_draw_animated::reset_animation(
	const ldtools::animation_table& _table
) {

	animation_table=&_table;
	return *this;
}

scenery_tile_draw_animated& scenery_tile_draw_animated::set_is_animation_fn(
	std::function<bool(int)> _fn
) {

	index_is_animation=_fn;
	return *this;
}

scenery_tile_draw_animated& scenery_tile_draw_animated::set_index_to_animation_fn(
	std::function<int(int)> _fn
) {

	index_to_animation=_fn;
	return *this;
}

void scenery_tile_draw_animated::draw(
	ldv::screen& _screen, 
	const scenery_tile& _tile
) {

	draw(_screen, _tile, internal_timer);
}

void scenery_tile_draw_animated::draw(
	ldv::screen& _screen, 
	const scenery_tile& _tile,
	float _timer
) {

	if(!index_is_animation || !index_is_animation(_tile.type)) {

		static_draw.draw(_screen, _tile);
		return;
	}

	if(!index_to_animation) {

		throw exception("cannot run scenery_tile_draw_animated if the index_to_animation function is not defined");
	}

	auto index=index_to_animation(_tile.type);
	const auto& animation=animation_table->get(index);
	const auto& frame=animation.get_for_time(_timer);
	int x=static_draw.get_tile_w()*_tile.x;
	int y=static_draw.get_tile_h()*_tile.y;

	return static_draw.sprite_draw.draw(_screen, {x, y}, frame.frame);
}

void scenery_tile_draw_animated::tic(
	float _delta
) {

	internal_timer+=_delta;
}

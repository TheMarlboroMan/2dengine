#include "d2d/video/scenery_tile_draw.h"
#include "d2d/video/exception.h"

using namespace d2d::video;

scenery_tile_draw::scenery_tile_draw(
	const ldtools::sprite_table& _sprite_table, 
	const ldv::texture& _texture,
	int _tile_w,
	int _tile_h,
	const ldtools::animation_table * _animation_table,
	ldv::camera * _camera,
	bool _with_camera
):
	sprite_draw{_sprite_table, _texture, _camera, _with_camera},
	tile_w{_tile_w},
	tile_h{_tile_h},
	animation_table{_animation_table}
{ }

void scenery_tile_draw::draw(
	ldv::screen& _screen,
	const scenery_tile& _tile
) {

	try {

		int x=tile_w*_tile.x;
		int y=tile_h*_tile.y;

		sprite_draw.draw(_screen, {x, y}, _tile.type);
	}
	catch(ldtools::sprite_table_exception& e) {

		throw exception(e.what());
	}
}

void scenery_tile_draw::draw_animation(
	ldv::screen& _screen, 
	const scenery_tile& _tile
) {

	draw_animation(_screen, _tile, internal_timer);
}

void scenery_tile_draw::draw_animation(
	ldv::screen& _screen, 
	const scenery_tile& _tile,
	ldtools::tdelta _timer
) {

	if(nullptr==query || !query->is_animation(_tile.type)) {

		draw(_screen, _tile);
		return;
	}

	auto index=query->get_animation(_tile.type);
	const auto& animation=animation_table->get(index);
	const auto& frame=animation.get_for_time(_timer);
	int x=tile_w*_tile.x;
	int y=tile_h*_tile.y;

	return sprite_draw.draw(_screen, {x, y}, frame.frame);
}


void scenery_tile_draw::tic(
	ldtools::tdelta _delta
) {

	internal_timer+=_delta;
}

scenery_tile_draw& scenery_tile_draw::set_scenery_tile_query_interface(
	scenery_tile_query_interface * _query
) {

	query=_query;
	return *this;
}

scenery_tile_draw& scenery_tile_draw::reset_animation_table(
	const ldtools::animation_table& _table
) {

	animation_table=&_table;
	return *this;
}


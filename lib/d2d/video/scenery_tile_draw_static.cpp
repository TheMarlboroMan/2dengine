#include "d2d/video/scenery_tile_draw_static.h"
#include "d2d/video/exception.h"

using namespace d2d::video;

scenery_tile_draw_static::scenery_tile_draw_static(
	const ldtools::sprite_table& _sprite_table, 
	const ldv::texture& _texture,
	int _tile_w,
	int _tile_h
):
	sprite_draw{_sprite_table, _texture},
	tile_w{_tile_w},
	tile_h{_tile_h}
{ }

void scenery_tile_draw_static::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	const scenery_tile& _tile
) {

	try {

		int x=tile_w*_tile.x;
		int y=tile_h*_tile.y;
		sprite_draw.draw(_screen, _camera, {x, y}, _tile.type);
	}
	catch(ldtools::sprite_table_exception& e) {

		throw exception(e.what());
	}
}

void scenery_tile_draw_static::draw(
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

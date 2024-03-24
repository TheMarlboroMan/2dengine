#include "d2d/video/scenery_tile_draw.h"
#include "d2d/video/exception.h"

using namespace d2d::video;

scenery_tile_draw::scenery_tile_draw(
	const ldtools::sprite_table& _sprite_table, 
	const ldv::texture& _texture,
	int _tile_w,
	int _tile_h,
	ldv::camera * _camera,
	bool _with_camera
):
	sprite_draw{_sprite_table, _texture, _camera, _with_camera},
	tile_w{_tile_w},
	tile_h{_tile_h}
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
